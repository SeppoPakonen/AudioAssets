#!/usr/bin/env python3
"""
Static site generator for /www based on escsrc/Timeline.

Generates:
- /www/index.html with Albums and Years from Timeline.upp
- /www/albums/<slug>/index.html from each album .upp
- /www/years/<year>/index.html from each year .upp (if present)
- /www/assets/{style.css,script.js}

The generator intentionally avoids external deps and keeps HTML simple for
older browsers. Modern browsers get progressive enhancement via CSS/JS.
"""
import os
import re
import html
from pathlib import Path
from urllib.parse import urlparse

ROOT = Path(__file__).resolve().parents[1]
ESC = ROOT / 'escsrc'
WWW = ROOT / 'www'
ASSETS = WWW / 'assets'
POLISHED = ('--polished' in os.sys.argv) or (os.environ.get('SITE_POLISHED') == '1')


def read_text(path: Path) -> str:
    return path.read_text(encoding='utf-8', errors='replace')


def ensure_dir(p: Path):
    p.mkdir(parents=True, exist_ok=True)


def write_text(path: Path, data: str):
    ensure_dir(path.parent)
    path.write_text(data, encoding='utf-8')


def parse_upp(path: Path):
    """Very small parser for .upp sections: description, file, uses."""
    text = read_text(path)
    desc_match = re.search(r'^description\s+"([^\n]*)";?', text, re.M)
    description = desc_match.group(1) if desc_match else None

    def parse_section(name: str):
        # Find 'name' then read until ';'
        m = re.search(rf'(?ms)^\s*{re.escape(name)}\s*\n(.*?);\s*\n', text)
        if not m:
            return []
        body = m.group(1)
        items = []
        for raw in body.splitlines():
            line = raw.strip()
            if not line:
                continue
            # Remove trailing comma
            if line.endswith(','):
                line = line[:-1]
            items.append(line)
        return items

    file_items_raw = parse_section('file')
    uses_items_raw = parse_section('uses')

    file_items = []
    for item in file_items_raw:
        if item.endswith('readonly separator'):
            label = item[:-len('readonly separator')].strip()
            file_items.append({'type': 'separator', 'label': label})
        else:
            # Strip wrapping quotes if any
            if item.startswith('"') and item.endswith('"'):
                value = item[1:-1]
            else:
                value = item
            file_items.append({'type': 'file', 'name': value})

    uses = []
    for item in uses_items_raw:
        val = item.strip()
        if val.startswith('"') and val.endswith('"'):
            val = val[1:-1]
        uses.append(val)

    return {
        'description': description,
        'files': file_items,
        'uses': uses,
        'raw': text,
    }


def extract_rgb(description: str | None):
    if not description:
        return None
    m = re.search(r'B(\d+),(\d+),(\d+)', description)
    if not m:
        return None
    r, g, b = (int(m.group(1)), int(m.group(2)), int(m.group(3)))
    # Clamp
    r = max(0, min(255, r)); g = max(0, min(255, g)); b = max(0, min(255, b))
    return (r, g, b)


def slugify(name: str) -> str:
    # Keep numeric prefix and words; lower-case; spaces/dots -> hyphens
    s = name.strip().lower()
    s = s.replace(".upp", "")
    s = re.sub(r'[^a-z0-9\s\-]+', '', s)
    s = re.sub(r'[\s]+', '-', s).strip('-')
    return s


def escape(s: str) -> str:
    return html.escape(s, quote=True)


def rel_from(page_dir: Path, target: Path) -> str:
    """Relative href from a page directory to a target file/dir (posix)."""
    rel = os.path.relpath(target, page_dir)
    return rel.replace('\\', '/')


def write_assets():
    style = r'''
/* Basic CSS for old browsers first */
html, body { margin: 0; padding: 0; }
body { font: 16px/1.4 Arial, Helvetica, sans-serif; color: #111; background: #f4f4f7; }
a { color: #0645ad; text-decoration: none; }
a:hover { text-decoration: underline; }
.wrap { max-width: 1100px; margin: 0 auto; padding: 12px; }
.header { padding: 16px 0; }
.title { font-size: 24px; font-weight: bold; }
.subtitle { color: #555; }
.main { background: linear-gradient(180deg, #f7f9ff, #f8f6ff); transition: background 2.5s ease; }
.main .wrap { background: linear-gradient(180deg, rgba(255,255,255,0.88), rgba(255,255,255,0.80)); }
.grid { /* float fallback */ }
.col { float: left; width: 100%; box-sizing: border-box; padding: 8px; }
.card { background: #fff; border: 1px solid #ddd; border-radius: 6px; padding: 12px; }
.muted { color: #666; }
.list { margin: 0; padding-left: 18px; }
.list li { margin: 4px 0; }
.card pre, pre.card, pre.card code { line-height: 1.2; }
.clearfix:after { content: ""; display: table; clear: both; }
.chip { display: inline-block; padding: 2px 6px; border: 1px solid #ccc; border-radius: 10px; font-size: 12px; color: #333; background: #fafafa; }
.sep { font-weight: bold; margin-top: 12px; border-top: 1px dashed #ddd; padding-top: 8px; }
.footer { margin: 24px 0; color: #777; font-size: 12px; }

/* Progressive enhancement */
@supports (display: grid) {
  .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 12px; }
  .col { float: none; width: auto; padding: 0; }
}

/* Fancy but safe background animation */
@media (prefers-reduced-motion: no-preference) {
  .banner {
    background: linear-gradient(120deg, #f0f3ff, #faf6ff);
    position: relative;
    overflow: hidden;
    transition: background 2.5s ease;
  }
  .banner:before {
    content: ""; position: absolute; inset: -40%;
    background: radial-gradient(120px 120px at 20% 30%, rgba(83, 109, 254, 0.08), transparent 60%),
                radial-gradient(140px 140px at 70% 40%, rgba(255, 64, 129, 0.06), transparent 60%),
                radial-gradient(180px 180px at 40% 80%, rgba(0, 200, 83, 0.05), transparent 60%);
    animation: floaty 16s linear infinite, huecycle 40s linear infinite;
    transition: background 3s ease;
    filter: hue-rotate(0deg);
  }
  @keyframes floaty {
    from { transform: translate3d(0,0,0) rotate(0deg); }
    to   { transform: translate3d(0,0,0) rotate(360deg); }
  }
  @keyframes huecycle {
    from { filter: hue-rotate(0deg); }
    to   { filter: hue-rotate(360deg); }
  }
}

.pill { padding: 3px 8px; border-radius: 999px; background: #eef; color: #334; font-size: 12px; border: 1px solid #ccd; }
.tag { font-size: 12px; color: #555; }
.nav { margin-top: 8px; }
.nav a { display: inline-block; margin-right: 8px; }
.section-title { margin: 8px 0; font-size: 18px; }
.album-grid { list-style: none; margin: 0; padding: 0; display: grid; grid-template-columns: repeat(auto-fill, minmax(220px, 1fr)); gap: 12px; }
.album-grid li { border: 1px solid #ddd; border-radius: 6px; background: #fff; padding: 10px; }
.small { font-size: 13px; }
.badge { padding: 2px 6px; border-radius: 8px; background: #f0f0f8; border: 1px solid #ddd; }
.prose p { margin: 0 0 8px 0; }
.prose h1, .prose h2, .prose h3 { margin: 10px 0 6px 0; }
.prose ul { margin: 0 0 8px 18px; }

/* Doc view toggle */
.doc-toggle { margin: 8px 0; }
.doc-toggle button { margin-right: 6px; padding: 4px 8px; }
.mode-rendered .doc-raw { display: none; }
.mode-raw .doc-rendered { display: none; }
'''

    script = r'''
(function(){
  // Add JS hook
  var cl = document.documentElement.classList; cl && cl.add('has-js');

  // Expand/collapse sections on modern browsers
  function toggleAll(selector, expand) {
    var nodes = document.querySelectorAll(selector);
    for (var i=0;i<nodes.length;i++) {
      var d = nodes[i];
      if ('open' in d) d.open = !!expand;
    }
  }
  var expander = document.getElementById('expand-all');
  var collapser = document.getElementById('collapse-all');
  if (expander) expander.onclick = function(){ toggleAll('details', true); };
  if (collapser) collapser.onclick = function(){ toggleAll('details', false); };

  // Query param helper
  function getParam(name){
    try {
      var u = new URL(window.location.href);
      return u.searchParams.get(name);
    } catch(e) { return null; }
  }

  // Doc render toggle
  var viewRendered = document.getElementById('view-rendered');
  var viewRaw = document.getElementById('view-raw');
  var body = document.body;
  function setDocMode(mode){
    if (!body) return;
    if (mode === 'rendered') { body.classList.add('mode-rendered'); body.classList.remove('mode-raw'); }
    else { body.classList.add('mode-raw'); body.classList.remove('mode-rendered'); }
    try { localStorage.setItem('aa_doc_mode', mode); } catch(e){}
  }
  if (viewRendered) viewRendered.onclick = function(){ setDocMode('rendered'); };
  if (viewRaw) viewRaw.onclick = function(){ setDocMode('raw'); };
  (function initDocMode(){
    if (!viewRendered && !viewRaw) return;
    var q = getParam('polish');
    var pref = null;
    try { pref = localStorage.getItem('aa_doc_mode'); } catch(e){}
    if (q === '1') setDocMode('rendered');
    else if (q === '0') setDocMode('raw');
    else if (pref) setDocMode(pref);
    else setDocMode('rendered');
  })();

  // Album filter: show only songs (md) and skip archive/agents/xx
  var filterBox = document.getElementById('filter-songs-only');
  function applyFilter(){
    var only = filterBox && filterBox.checked;
    var items = document.querySelectorAll('ul.list li.file-item');
    for (var i=0;i<items.length;i++){
      var it = items[i];
      var tag = it.getAttribute('data-tag') || '';
      var name = (it.getAttribute('data-name') || '').toLowerCase();
      var hide = false;
      if (only){
        if (tag !== 'doc') hide = true;
        if (!hide) {
          if (name.startsWith('xx ')) hide = true;
          if (name.indexOf('eng archive') === 0 || name.indexOf('fin archive') === 0) hide = true;
          if (name === 'agents.md' || name === 'current_task.md') hide = true;
        }
      }
      it.style.display = hide ? 'none' : '';
    }
    try { localStorage.setItem('aa_album_filter', only ? '1':'0'); } catch(e){}
  }
  if (filterBox){
    filterBox.onchange = applyFilter;
    var pref = null; try { pref = localStorage.getItem('aa_album_filter'); } catch(e){}
    if (getParam('polish') === '1') filterBox.checked = true;
    else if (pref) filterBox.checked = (pref === '1');
    applyFilter();
  }

  // Theme handler: apply banner colors (modern browsers)
  function applyTheme(r,g,b){
    function hex2(x){ return x.toString(16).padStart(2,'0'); }
    function tint(x,t){ return Math.round(x + (255 - x) * t); }
    var bg1 = '#' + hex2(tint(r,0.85)) + hex2(tint(g,0.85)) + hex2(tint(b,0.85));
    var bg2 = '#' + hex2(tint(r,0.70)) + hex2(tint(g,0.70)) + hex2(tint(b,0.70));
    var c1 = 'rgba(' + tint(r,0.10) + ',' + tint(g,0.10) + ',' + tint(b,0.10) + ',0.14)';
    var c2 = 'rgba(' + tint(r,0.25) + ',' + tint(g,0.25) + ',' + tint(b,0.25) + ',0.10)';
    var c3 = 'rgba(' + tint(r,0.40) + ',' + tint(g,0.40) + ',' + tint(b,0.40) + ',0.08)';
    var css = '.banner{background:linear-gradient(120deg,'+bg1+','+bg2+');}' +
              '.banner:before{background:radial-gradient(120px 120px at 20% 30%,'+c1+', transparent 60%),' +
              'radial-gradient(140px 140px at 70% 40%,'+c2+', transparent 60%),' +
              'radial-gradient(180px 180px at 40% 80%,'+c3+', transparent 60%);}'+
              '.main{background:linear-gradient(180deg,'+bg1+','+bg2+');}';
    var el = document.getElementById('aa-theme');
    if (!el){ el = document.createElement('style'); el.id = 'aa-theme'; document.head.appendChild(el); }
    el.textContent = css;
  }

  function startGradientLoop(baseR, baseG, baseB){
    try { if (window.matchMedia && window.matchMedia('(prefers-reduced-motion: reduce)').matches) return; } catch(e){}
    function clamp(x){ return Math.max(0, Math.min(255, x)); }
    function jitter(x, amt){ return clamp(x + Math.round((Math.random()*2-1)*amt)); }
    setInterval(function(){
      var rr = jitter(baseR, 20), gg = jitter(baseG, 20), bb = jitter(baseB, 20);
      applyTheme(rr, gg, bb);
    }, 8000);
  }

  // Apply album theme if provided via data attrs; else random on home page
  (function(){
    var bodyEl = document.body;
    if (!bodyEl) return;
    var r = bodyEl.getAttribute('data-base-r');
    var g = bodyEl.getAttribute('data-base-g');
    var b = bodyEl.getAttribute('data-base-b');
    var page = bodyEl.getAttribute('data-page');
    if (r && g && b){ var br=parseInt(r,10), bg=parseInt(g,10), bb=parseInt(b,10); applyTheme(br, bg, bb); startGradientLoop(br,bg,bb); return; }
    if (page === 'home'){
      // Random pleasant base per load
      var rr = Math.floor(Math.random()*156)+50; // 50-205
      var gg = Math.floor(Math.random()*156)+50;
      var bb = Math.floor(Math.random()*156)+50;
      applyTheme(rr,gg,bb);
      startGradientLoop(rr,gg,bb);
    }
  })();
})();
'''

    write_text(ASSETS / 'style.css', style)
    write_text(ASSETS / 'script.js', script)


def render_markdown_basic(md_text: str) -> str:
    """Small, dependency-free Markdown renderer.
    Block support:
    - Headings (# .. ######)
    - Unordered lists (- item)
    - Code fences (``` ... ```)
    - Horizontal rule (--- or ***)
    - Paragraphs
    Inline support:
    - Links [text](url)
    - Autolinks <http://...>
    - Images ![alt](url) — special case: if URL contains .mp3, render as a link instead of image
    - Code spans `code`
    - Emphasis: *em* _em_, **strong** __strong__ (basic)
    """
    def render_inlines(text: str) -> str:
        # Protect code spans first
        code_spans = []
        def code_sub(m):
            idx = len(code_spans)
            code_spans.append('<code>' + escape(m.group(1)) + '</code>')
            return f"\u0000C{idx}\u0000"
        text2 = re.sub(r'`([^`]+)`', code_sub, text)

        # Images -> if mp3, render as link; else <img>
        def img_sub(m):
            alt, url = m.group(1), m.group(2)
            if '.mp3' in url.lower():
                return '<a href="' + escape(url) + '">' + escape(alt or 'Audio') + '</a>'
            return '<img class="md-img" src="' + escape(url) + '" alt="' + escape(alt) + '">' 
        text2 = re.sub(r'!\[([^\]]*)\]\(([^)\s]+)\)', img_sub, text2)

        # Links [text](url)
        def link_sub(m):
            label, url = m.group(1), m.group(2)
            return '<a href="' + escape(url) + '">' + escape(label) + '</a>'
        text2 = re.sub(r'\[([^\]]+)\]\(([^)\s]+)\)', link_sub, text2)

        # Autolinks <http://...>
        text2 = re.sub(r'<(https?://[^>\s]+)>', lambda m: '<a href="' + escape(m.group(1)) + '">' + escape(m.group(1)) + '</a>', text2)

        # Protect produced HTML tags from inline emphasis replacement
        html_tokens = []
        def protect_html(m):
            idx = len(html_tokens)
            html_tokens.append(m.group(0))
            return f"\u0000H{idx}\u0000"
        text2 = re.sub(r'<a [^>]*?>.*?</a>', protect_html, text2)
        text2 = re.sub(r'<img [^>]*?>', protect_html, text2)

        # Bold then italics (very basic, non-nested)
        text2 = re.sub(r'\*\*([^*]+)\*\*', lambda m: '<strong>' + escape(m.group(1)) + '</strong>', text2)
        text2 = re.sub(r'__([^_]+)__', lambda m: '<strong>' + escape(m.group(1)) + '</strong>', text2)
        text2 = re.sub(r'\*([^*]+)\*', lambda m: '<em>' + escape(m.group(1)) + '</em>', text2)
        text2 = re.sub(r'_([^_]+)_', lambda m: '<em>' + escape(m.group(1)) + '</em>', text2)

        # Restore protected HTML
        def restore_html(s: str) -> str:
            def rep(m):
                idx = int(m.group(1))
                return html_tokens[idx]
            return re.sub(r'\x00H(\d+)\x00', rep, s)
        text2 = restore_html(text2)

        # Restore code spans
        def restore_codes(s: str) -> str:
            def rep(m):
                idx = int(m.group(1))
                return code_spans[idx]
            return re.sub(r'\x00C(\d+)\x00', rep, s)
        return restore_codes(text2)

    lines = md_text.splitlines()
    out = []
    in_code = False
    in_list = False
    para_buf = []
    code_buf = []

    def flush_para():
        nonlocal para_buf
        if para_buf:
            text = ' '.join(para_buf).strip()
            if text:
                out.append('<p>' + render_inlines(text) + '</p>')
            para_buf = []

    def close_list():
        nonlocal in_list
        if in_list:
            out.append('</ul>')
            in_list = False

    for line in lines:
        if line.startswith('```'):
            if in_code:
                # Flush code block
                content = '\n'.join(code_buf)
                # Collapse multiple blank lines to a single blank line
                content = re.sub(r'\n{2,}', '\n', content).strip('\n')
                out.append('<pre class="card"><code>' + escape(content) + '</code></pre>')
                in_code = False
                code_buf = []
            else:
                flush_para()
                close_list()
                in_code = True
                code_buf = []
            continue
        if in_code:
            code_buf.append(line)
            continue
        if not line.strip():
            flush_para()
            close_list()
            continue
        m = re.match(r'^(#{1,6})\s+(.*)$', line)
        if m:
            flush_para()
            close_list()
            level = len(m.group(1))
            out.append(f'<h{level}>' + render_inlines(m.group(2).strip()) + f'</h{level}>')
            continue
        # Horizontal rule
        if re.match(r'^\s*([-*_])\1\1+\s*$', line):
            flush_para(); close_list()
            out.append('<hr>')
            continue
        if re.match(r'^\s*-\s+', line):
            flush_para()
            if not in_list:
                out.append('<ul class="list">')
                in_list = True
            item = re.sub(r'^\s*-\s+', '', line)
            out.append('<li>' + render_inlines(item) + '</li>')
            continue
        para_buf.append(line.strip())

    if in_code:
        out.append('</code></pre>')
    flush_para()
    close_list()
    return '\n'.join(out)


def slugify_basename(file_name: str) -> str:
    base = os.path.splitext(os.path.basename(file_name))[0]
    return slugify(base)


def extract_mp3_links(md_text: str):
    """Return list of (url, label) for any mp3 links/images/autolinks found."""
    results = []
    # Markdown image: ![alt](url)
    for m in re.finditer(r'!\[([^\]]*)\]\(([^)\s]+)\)', md_text):
        alt, url = m.group(1), m.group(2)
        if '.mp3' in url.lower():
            results.append((url, alt or 'Audio'))
    # Markdown link: [text](url)
    for m in re.finditer(r'\[([^\]]*)\]\(([^)\s]+)\)', md_text):
        text, url = m.group(1), m.group(2)
        if '.mp3' in url.lower():
            results.append((url, text or 'Audio'))
    # Autolink: <https://...>
    for m in re.finditer(r'<(https?://[^>\s]+)>', md_text):
        url = m.group(1)
        if '.mp3' in url.lower():
            results.append((url, 'Audio'))
    # Bare URL: https://...
    for m in re.finditer(r'(?<![\(\<])\bhttps?://\S+', md_text):
        url = m.group(0)
        if '.mp3' in url.lower():
            results.append((url, 'Audio'))
    # Deduplicate
    seen = set()
    dedup = []
    for url, label in results:
        if url not in seen:
            seen.add(url)
            dedup.append((url, label))
    return dedup


def gen_md_doc_page(album_name: str, album_slug: str, md_path: Path):
    title = f"{os.path.splitext(md_path.name)[0]} · {album_name}"
    md_text = read_text(md_path)
    players = []
    for url, label in extract_mp3_links(md_text):
        safe = escape(url)
        lab = escape(label)
        players.append(
            f'<div class="card"><div class="small muted">{lab}</div>'
            f'<audio controls preload="none" src="{safe}"><a href="{safe}">{lab} (MP3)</a></audio>'
            f'</div>'
        )
    players_html = '\n'.join(players) if players else '<div class="card small muted">No MP3 links found in this document.</div>'

    # Markdown (rendered in polished mode) or raw fallback
    rendered = render_markdown_basic(md_text)
    raw_block = f'<pre class="doc doc-raw card small">{escape(md_text)}</pre>'
    rendered_block = f'<div class="doc doc-rendered">{rendered}</div>'

    src_rel = rel_from(WWW / 'albums' / album_slug / slugify_basename(md_path.name), md_path)
    toggle_ui = '<div class="doc-toggle small nav"><button id="view-rendered" class="badge">Rendered</button> <button id="view-raw" class="badge">Raw</button></div>'
    body = (
        f'<div class="small muted">Source: <a href="{escape(src_rel)}">{escape(md_path.name)}</a></div>'
        f'<div class="section-title">Audio</div>'
        f'{players_html}'
        f'<div class="section-title">Document</div>'
        f'{toggle_ui}'
        f'{rendered_block}{raw_block}'
    )
    # Theme from album upp (same as album page)
    theme_rgb = None
    upp_path = md_path.parent / (album_name + '.upp')
    if upp_path.exists():
        try:
            data = parse_upp(upp_path)
            theme_rgb = extract_rgb(data.get('description'))
        except Exception:
            pass
    return layout(title, body, breadcrumbs=[('Home', '/'), (album_name, f'/albums/{album_slug}/'), (os.path.splitext(md_path.name)[0], None)], base_prefix='../../../', theme_rgb=theme_rgb, page_id='doc')


def layout(title: str, body_html: str, subtitle: str = None, breadcrumbs=None, base_prefix: str = '', theme_rgb=None, page_id: str = None):
    bc_html = ''
    if breadcrumbs:
        bc = []
        for text, href in breadcrumbs:
            if href:
                if href.startswith('/'):
                    bc.append(f'<a href="{escape(href)}">{escape(text)}</a>')
                else:
                    bc.append(f'<a href="{escape(base_prefix + href)}">{escape(text)}</a>')
            else:
                bc.append(f'<span class="muted">{escape(text)}</span>')
        bc_html = '<div class="nav">' + ' / '.join(bc) + '</div>'
    sub = f'<div class="subtitle">{escape(subtitle)}</div>' if subtitle else ''
    # Inline theme CSS based on RGB
    theme_style = ''
    if theme_rgb:
        r, g, b = theme_rgb
        # build light tints for background and soft blobs
        def to_hex(v):
            return f"{v:02x}"
        def tint(x, t):
            return int(round(x + (255 - x) * t))
        bg1 = f"#{to_hex(tint(r,0.85))}{to_hex(tint(g,0.85))}{to_hex(tint(b,0.85))}"
        bg2 = f"#{to_hex(tint(r,0.70))}{to_hex(tint(g,0.70))}{to_hex(tint(b,0.70))}"
        c1 = f"rgba({tint(r,0.10)}, {tint(g,0.10)}, {tint(b,0.10)}, 0.14)"
        c2 = f"rgba({tint(r,0.25)}, {tint(g,0.25)}, {tint(b,0.25)}, 0.10)"
        c3 = f"rgba({tint(r,0.40)}, {tint(g,0.40)}, {tint(b,0.40)}, 0.08)"
        # Also compute a vertical main gradient
        mg1 = f"#{to_hex(tint(r,0.93))}{to_hex(tint(g,0.93))}{to_hex(tint(b,0.93))}"
        mg2 = f"#{to_hex(tint(r,0.80))}{to_hex(tint(g,0.80))}{to_hex(tint(b,0.80))}"
        theme_style = f"""
<style>
.banner{{background: linear-gradient(120deg, {bg1}, {bg2});}}
.banner:before{{
  background: radial-gradient(120px 120px at 20% 30%, {c1}, transparent 60%),
              radial-gradient(140px 140px at 70% 40%, {c2}, transparent 60%),
              radial-gradient(180px 180px at 40% 80%, {c3}, transparent 60%);
}}
.main{{background: linear-gradient(180deg, {mg1}, {mg2});}}
</style>
"""
    # Body attributes for JS theme (front page random)
    body_attrs = ''
    if page_id:
        body_attrs += f' data-page="{page_id}"'
    if theme_rgb and not page_id:
        page_id = 'page'
    if theme_rgb:
        r, g, b = theme_rgb
        body_attrs += f' data-base-r="{r}" data-base-g="{g}" data-base-b="{b}"'

    return f'''<!doctype html>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{escape(title)}</title>
<link rel="stylesheet" href="{escape(base_prefix)}assets/style.css">
{theme_style}
<body{body_attrs}>
  <div class="banner">
    <div class="wrap header">
      <div class="title">AudioAssets Timeline</div>
      {sub}
      {bc_html}
    </div>
  </div>
  <div class="main">
    <div class="wrap">
      {body_html}
      <div class="footer">Static site generated from escsrc/Timeline. No server-side code. <span class="muted small">Works without JS; modern browsers get subtle animations.</span></div>
    </div>
  </div>
<script src="{escape(base_prefix)}assets/script.js"></script>
'''


def gen_index(years, albums, front_html: str = None):
    # Left: front_html (rendered Markdown), Right: albums
    items = []
    for name in albums:
        slug = slugify(name)
        items.append(f'<li><div class="small muted">Album</div><div><a href="albums/{escape(slug)}/">{escape(name)}</a></div></li>')
    album_html = '<ul class="album-grid">' + '\n'.join(items) + '</ul>'

    left = '<div class="prose">' + (front_html or '<p class="small muted">Browse albums to explore the timeline.</p>') + '</div>'

    body = f'''
<div class="grid clearfix">
  <div class="col card">{left}</div>
  <div class="col card">
    <div class="section-title">Albums</div>
    {album_html}
  </div>
</div>
'''
    return layout('AudioAssets Timeline', body, subtitle='Albums overview', breadcrumbs=None, base_prefix='', page_id='home')


def gen_album_page(album_name: str, upp_path: Path):
    data = parse_upp(upp_path)
    sections = []
    current = []
    current_label = None
    album_dir = upp_path.parent
    album_slug = slugify(album_name)
    page_dir = WWW / 'albums' / album_slug
    # Optional album About.md rendered at top
    about_html = None
    for cand in (album_dir / 'About.md', album_dir / 'ABOUT.md', album_dir / 'about.md'):
        if cand.exists():
            try:
                about_html = render_markdown_basic(read_text(cand))
            except Exception:
                about_html = '<p class="small muted">About.md could not be rendered.</p>'
            break
    def is_irrelevant_md(name: str) -> bool:
        low = name.lower()
        if low.startswith('xx '):
            return True
        if low.startswith('eng archive') or low.startswith('fin archive'):
            return True
        if low in ('agents.md', 'current_task.md'):
            return True
        return False

    for entry in data['files']:
        if entry['type'] == 'separator':
            # Flush previous
            if current:
                sections.append((current_label or 'Files', current))
                current = []
            current_label = entry['label']
        else:
            name = entry['name']
            ext = os.path.splitext(name)[1].lower()
            tag = {
                '.tg': 'score',
                '.md': 'doc',
                '.txt': 'text',
                '.ecs': 'meta',
            }.get(ext, ext[1:] if ext else 'file')
            src = album_dir / name
            display = escape(name)
            if POLISHED and ext != '.md':
                # In polished mode, only keep Markdown song/docs
                continue
            if ext == '.md' and POLISHED and is_irrelevant_md(name):
                continue
            if ext == '.md':
                # Build a per-song doc page that embeds MP3 players
                song_slug = slugify_basename(name)
                doc_dir = page_dir / song_slug
                doc_html = gen_md_doc_page(album_name, album_slug, src)
                write_text(doc_dir / 'index.html', doc_html)
                href = f'{song_slug}/'
            else:
                href = rel_from(page_dir, src)
            current.append((display, href, tag))
    if current:
        sections.append((current_label or 'Files', current))

    blocks = []
    if about_html:
        blocks.append('<div class="section-title">About</div>')
        blocks.append('<div class="card prose">' + about_html + '</div>')
    blocks.append('<div class="small muted">Source folder: ' + escape(rel_from(page_dir, album_dir)) + '</div>')
    blocks.append('<div class="small nav"><button id="expand-all" class="badge">Expand all</button> <button id="collapse-all" class="badge">Collapse all</button> <label style="margin-left:8px;"><input type="checkbox" id="filter-songs-only"> Show only songs</label></div>')

    for label, items in sections:
        # details open by default for old browsers (which ignore <details>)
        rows = []
        for disp, href, tag in items:
            rows.append(f'<li class="file-item" data-tag="{escape(tag)}" data-name="{disp}"><a href="{escape(href)}">{disp}</a> <span class="tag">[{escape(tag)}]</span></li>')
        ul = '<ul class="list">' + '\n'.join(rows) + '</ul>'
        blocks.append(f'<details open><summary class="sep">{escape(label)}</summary>{ul}</details>')

    body = '\n'.join(blocks)
    theme_rgb = extract_rgb(data.get('description'))
    return layout(f'{album_name} · Album', body, breadcrumbs=[('Home', '/'), (album_name, None)], base_prefix='../../', theme_rgb=theme_rgb, page_id='album')


def gen_year_page(year: str, upp_path: Path):
    data = parse_upp(upp_path)
    sections = []
    current = []
    current_label = None
    year_dir = upp_path.parent
    page_dir = WWW / 'years' / year
    for entry in data['files']:
        if entry['type'] == 'separator':
            if current:
                sections.append((current_label or 'Files', current))
                current = []
            current_label = entry['label']
        else:
            name = entry['name']
            ext = os.path.splitext(name)[1].lower()
            tag = {
                '.tg': 'score',
                '.md': 'doc',
                '.txt': 'text',
                '.ecs': 'meta',
            }.get(ext, ext[1:] if ext else 'file')
            src = year_dir / name
            href = rel_from(page_dir, src)
            display = escape(name)
            current.append((display, href, tag))
    if current:
        sections.append((current_label or 'Files', current))

    blocks = []
    blocks.append('<div class="small muted">Source folder: ' + escape(rel_from(page_dir, year_dir)) + '</div>')
    for label, items in sections:
        rows = []
        for disp, href, tag in items:
            rows.append(f'<li><a href="{escape(href)}">{disp}</a> <span class="tag">[{escape(tag)}]</span></li>')
        ul = '<ul class="list">' + '\n'.join(rows) + '</ul>'
        blocks.append(f'<details open><summary class="sep">{escape(label)}</summary>{ul}</details>')

    body = '\n'.join(blocks)
    return layout(f'{year} · Year', body, breadcrumbs=[('Home', '/'), (year, None)], base_prefix='../../')


def main():
    timeline_upp = ESC / 'Timeline' / 'Timeline.upp'
    if not timeline_upp.exists():
        raise SystemExit('Timeline.upp not found')
    tl = parse_upp(timeline_upp)

    # Split uses into years and albums
    years = []
    albums = []
    for u in tl['uses']:
        if re.fullmatch(r'\d{4}', u):
            years.append(u)
        else:
            albums.append(u)

    # Assets
    write_assets()

    # Optional front-page Markdown content from Timeline
    front_md = ESC / 'Timeline' / 'Front.md'
    front_html = None
    if front_md.exists():
        try:
            front_html = render_markdown_basic(read_text(front_md))
        except Exception:
            front_html = '<p class="small muted">Front.md could not be rendered.</p>'

    # Index
    write_text(WWW / 'index.html', gen_index(years, albums, front_html))

    # Album pages
    for name in albums:
        folder = ESC / name
        upp = folder / (name + '.upp')
        if not upp.exists():
            # Try best-effort: some folders use exact casing; otherwise skip
            print(f'WARN: missing .upp for album {name!r} at {upp}')
            continue
        html_path = WWW / 'albums' / slugify(name) / 'index.html'
        write_text(html_path, gen_album_page(name, upp))

    # Year pages
    for y in years:
        folder = ESC / y
        upp = folder / (y + '.upp')
        if not upp.exists():
            print(f'WARN: missing .upp for year {y} at {upp}')
            continue
        html_path = WWW / 'years' / y / 'index.html'
        write_text(html_path, gen_year_page(y, upp))

    # 404 (optional)
    write_text(WWW / '404.html', layout('Not found', '<p class="card">Page not found.</p>', breadcrumbs=[('Home', ''), ('404', None)], base_prefix=''))

    print('Generated site at', WWW)


if __name__ == '__main__':
    main()
