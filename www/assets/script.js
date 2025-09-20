
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
