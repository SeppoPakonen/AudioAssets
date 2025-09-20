
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
})();
