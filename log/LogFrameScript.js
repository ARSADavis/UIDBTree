function additiveLog(data) {
    let offset = sessionStorage.getItem('offset');
    if (offset == null) {
        window.parent.postMessage(data, '*');
    } else {
        window.parent.postMessage(data.substring(offset), '*');
    }
    sessionStorage.setItem('offset', data.length);
}