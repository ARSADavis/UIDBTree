let loadedFile = function(fileContents) {
    logFileContents.innerHTML = fileContents;
};

let refreshLogFunction = function() {
    if (!refreshLogCheckbox.checked) {
        return;
    }
    refreshLogTime = 1000;
    if (logFileInput.files[0] != null) {
        fileReader.readAsText(logFileInput.files[0]);
    } else {
        logFileContents.innerHTML = 'No log file selected!';
        clearInterval(refreshLogInterval);
    }
};

let refreshLogTimeFunction = function() {
    refreshLogTime -= 100;
    refreshLogLabel.innerHTML = 'Refresh Log (' + (Math.round(refreshLogTime) / 1000.0).toFixed(1) + ')';
};

window.updateLogRefresh = function() {
    if (refreshLogCheckbox.checked) {
        refreshLogFunction();
        refreshLogInterval = setInterval(refreshLogFunction, 1000);
        refreshLogTimeInterval = setInterval(refreshLogTimeFunction, 100);
    } else {
        clearInterval(refreshLogInterval);
        clearInterval(refreshLogTimeInterval);
        refreshLogLabel.innerHTML = 'Refresh Log';
    }
};

window.onload = function() {
    window.clearLogButton = document.getElementById('clearLogButton');
    window.logFileInput = document.getElementById('logFileInput');
    window.logFileContents = document.getElementById('logFileContents');
    window.refreshLogCheckbox = document.getElementById('refreshLogCheckbox');
    window.refreshLogLabel = document.getElementById('refreshLogLabel');
    window.refreshLogInterval = null;
    window.refreshLogTime = 0;
    window.refreshLogTimeInterval = null;
    window.fileReader = new FileReader();
    fileReader.onloadend = function() {
        loadedFile(fileReader.result);
    };

    clearLogButton.onclick = function() {
        refreshLogCheckbox.checked = false;
        logFileInput.value = '';
        logFileContents.innerHTML = 'Cleared!';
    };

    refreshLogCheckbox.onclick = updateLogRefresh;
    updateLogRefresh();
};