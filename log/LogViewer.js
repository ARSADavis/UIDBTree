const refreshTimeUpdateInterval = 10;
const refreshInterval = 1000;

window.refreshLogInterval = null;
window.refreshLogTime = 0;

let reset = function(options = {}) {
    if (options.resetOffset) {
        sessionStorage.setItem('offset', 0);
    }
    if (options.resetLogInterval) {
        clearLogInterval();
    }
    if (options.resetCheckboxOff) {
        HTML.autoRefreshCheckbox.checked = false;
    } else if (options.resetCheckboxOn) {
        HTML.autoRefreshCheckbox.checked = false;
        HTML.autoRefreshCheckbox.click();
    }
    if (options.resetFile) {
        HTML.logFileInput.value = '';
        HTML.logFileInputLabel.innerHTML = '';
    }
    if (options.resetLogContents) {
        HTML.logContents.innerHTML = '';
    } else if (options.clearLogContents) {
        HTML.logContents.innerHTML = 'Cleared!';
    }
    if (options.resetFrame) {
        HTML.logFileFrame.src = '';
    }
};

let onRefresh = function() {
    refreshLogTime -= refreshTimeUpdateInterval;
    HTML.refreshLogLabel.innerHTML = ' (' + (Math.round(refreshLogTime) / 1000.0).toFixed(1) + 's)';
    if (refreshLogTime <= 0) {
        refreshLog();
    }
};

let reloadFrame = function() {
    if (HTML.logFileInput.files[0] == null) {
        HTML.logFileFrame.src = '';
        HTML.logContents.innerHTML = '';
    } else {
        //Get rid of 'C:\\fakepath\\'...
        HTML.logFileFrame.src = HTML.logFileInput.value.substring(12);
    }
};

let refreshLog = function() {
    refreshLogTime = refreshInterval;
    reloadFrame();
};

let clearLogInterval = function() {
    clearInterval(refreshLogInterval);
    refreshLogInterval = null;
    HTML.refreshLogLabel.innerHTML = '';
};

let fullReload = function() {
    reset({
        resetOffset: true,
        resetLogInterval: true,
        resetCheckboxOn: true,
        resetLogContents: true,
        resetFrame: true
    });
    reloadFrame();
};

window.onmessage = function(e) {
    HTML.logContents.innerHTML += e.data;
};

window.onload = function() {
    //Get the HTML elements.
    window.HTML = document.body.getElementsByTagName('*');

    HTML.logFileInput.onchange = function() {
        //Get rid of 'C:\\fakepath\\'...
        HTML.logFileInputLabel.innerHTML = HTML.logFileInput.value.substring(12);
        fullReload();
    };
    HTML.fullReloadButton.onclick = function() {
        if (HTML.logFileInput.files[0] == null) {
            HTML.logContents.innerHTML = 'Choose a log file first!';
            return;
        }
        fullReload();
    };

    HTML.autoRefreshCheckbox.onclick = function() {
        if (HTML.autoRefreshCheckbox.checked) {
            if (HTML.logFileInput.files[0] == null) {
                HTML.logContents.innerHTML = 'Choose a log file first!';
                return;
            }
            refreshLogTime = refreshTimeUpdateInterval;
            onRefresh();
            refreshLogInterval = setInterval(onRefresh, refreshTimeUpdateInterval);
        } else {
            clearLogInterval();
        }
    };

    HTML.resetConfigurationButton.onclick = reset.bind(this, {
        resetOffset: true,
        resetLogInterval: true,
        resetCheckboxOff: true,
        resetFile: true,
        resetFrame: true
    });

    HTML.clearLogButton.onclick = reset.bind(this, {
        resetOffset: true,
        resetLogInterval: true,
        resetCheckboxOff: true,
        clearLogContents: true
    });

    reset({
        resetOffset: true,
        resetLogInterval: true,
        resetCheckboxOff: true,
        resetFile: true,
        resetLogContents: true,
        resetFrame: true
    });
};