const refreshTimeUpdateInterval = 100;
const refreshInterval = 3000;

window.refreshLogInterval = null;
window.refreshLogTime = 0;

window.isResetting = false;
let reset = function(options = {}) {
    let previousIsResetting = isResetting;
    isResetting = true;
    if (options.resetOffset) {
        sessionStorage.setItem('offset', 0);
    }
    if (options.resetCheckboxOff) {
        HTML.autoRefreshCheckbox.checked = false;
        HTML.autoRefreshCheckbox.dispatchEvent(new Event('change'));
    } else if (options.resetCheckboxOn) {
        HTML.autoRefreshCheckbox.checked = true;
        HTML.autoRefreshCheckbox.dispatchEvent(new Event('change'));
    }
    if (options.resetFile) {
        window.filePath = '';
        HTML.logFileInput.value = window.filePath;
        HTML.logFileInputLabel.innerHTML = window.filePath;
    }
    if (options.resetLogContents) {
        HTML.logContents.innerHTML = '';
    } else if (options.clearLogContents) {
        HTML.logContents.innerHTML = 'Cleared!';
    }
    if (options.resetFrame) {
        HTML.logFileFrame.src = '';
    }
    isResetting = previousIsResetting;
};

let onRefresh = function() {
    refreshLogTime -= refreshTimeUpdateInterval;
    HTML.refreshLogLabel.innerHTML = ' (' + (Math.round(refreshLogTime) / 1000.0).toFixed(1) + 's)';
    if (refreshLogTime <= 0) {
        refreshLog();
    }
};

let reloadFrame = function() {
    //Get rid of 'C:\\fakepath\\'...
    HTML.logFileFrame.src = window.filePath;
};

let refreshLog = function() {
    refreshLogTime = refreshInterval;
    if (!isResetting) {
        reloadFrame();
    }
};

let clearLogInterval = function() {
    clearInterval(refreshLogInterval);
    refreshLogInterval = null;
    HTML.refreshLogLabel.innerHTML = '';
};

let fullReload = function() {
    reset({
        resetOffset: true,
        resetCheckboxOn: true,
        resetLogContents: true
    });
    reloadFrame();
};

let onAutoRefresh = function() {
    if (HTML.autoRefreshCheckbox.checked) {
        clearLogInterval();
        refreshLogTime = refreshTimeUpdateInterval;
        onRefresh();
        refreshLogInterval = setInterval(onRefresh, refreshTimeUpdateInterval);
    } else {
        clearLogInterval();
    }
};

window.onmessage = function(e) {
    if (e.data == null) {
        reset({ resetLogContents: true });
        return;
    }
    let newOutputSpan;
    for (let outputSegment of e.data) {
        newOutputSpan = document.createElement('span');
        newOutputSpan.innerHTML = outputSegment;
        HTML.logContents.prepend(newOutputSpan);
    }
};

window.onload = function() {
    //Get the HTML elements.
    window.HTML = document.body.getElementsByTagName('*');

    HTML.logFileInput.onchange = function() {
        if (isResetting) {
            return;
        }
        //Get rid of 'C:\\fakepath\\'...
        window.filePath = HTML.logFileInput.value.substring(12);
        HTML.logFileInputLabel.innerHTML = window.filePath;
        fullReload();
    };
    HTML.fullReloadButton.onclick = function() {
        fullReload();
    };

    HTML.autoRefreshCheckbox.onchange = onAutoRefresh;

    HTML.resetButton.onclick = reset.bind(this, {
        resetOffset: true,
        resetCheckboxOff: true,
        resetFile: true,
        resetFrame: true
    });

    HTML.clearLogButton.onclick = reset.bind(this, {
        resetOffset: true,
        resetCheckboxOff: true,
        clearLogContents: true
    });

    window.navigationType = performance.getEntriesByType('navigation')[0].type;

    window.filePath = 'log.html';
    HTML.logFileInputLabel.innerHTML = window.filePath;
    switch (navigationType) {
        case 'navigate':
            break;
        default:
            reset({ resetOffset: true });
            fullReload();
    }
    onAutoRefresh();
};