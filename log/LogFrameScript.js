window.logStartTime = '';
window.logEndTime = '';
window.loggingFinished = false;
window.output = [];
window.onload = function() {
    let lastLogStartTime = sessionStorage.getItem('lastLogStartTime');
    let offset;
    if (logStartTime != lastLogStartTime) {
        offset = null;
        window.parent.postMessage('reset', '*');
    } else {
        offset = sessionStorage.getItem('offset');
    }
    if (offset == null) {
        window.parent.postMessage(output, '*');
        sessionStorage.setItem('offset', output.length);
    } else if (output.length > offset) {
        window.parent.postMessage(output.slice(offset), '*');
        sessionStorage.setItem('offset', output.length);
    }
    sessionStorage.setItem('lastLogStartTime', logStartTime);
};