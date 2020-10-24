window.output = [];
window.onload = function() {
    let offset = sessionStorage.getItem('offset');
    if (offset == null) {
        window.parent.postMessage(output, '*');
        sessionStorage.setItem('offset', output.length);
    } else if (output.length > offset) {
        window.parent.postMessage(output.slice(offset), '*');
        sessionStorage.setItem('offset', output.length);
    }
};