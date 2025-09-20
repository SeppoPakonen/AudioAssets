
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
})();
