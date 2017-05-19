const {app, BrowserWindow} = require('electron');
const path = require('path');
const url = require('url');
const LocalStorage = require('node-localstorage').LocalStorage;

let mainWindow;
let localStorage = new LocalStorage('./config');

function recall(key, default_value) {
  let value = localStorage.getItem(key);
  return value === null ? default_value : value;
}

function recallInt(key, default_value) {
  let value = localStorage.getItem(key);
  return value === null ? default_value : parseInt(value);
}

function createWindow() {
  // Restore settings or their defaults.
  let width = recallInt('window_width', 800);
  let height = recallInt('window_height', 600);
  let showTools = recallInt('show_tools', 0) == 1;

  mainWindow = new BrowserWindow({
    width: width,
    height: height
  })

  mainWindow.loadURL(url.format({
    pathname: path.join(__dirname, 'index.html'),
    protocol: 'file:',
    slashes: true
  }))

  if (showTools) {
    mainWindow.webContents.openDevTools({mode: 'bottom'});
  }

  mainWindow.on('close', () => {
    let size = mainWindow.getSize();
    localStorage.setItem('window_width', size[0]);
    localStorage.setItem('window_height', size[1]);
    localStorage.setItem('show_tools', mainWindow.webContents.isDevToolsOpened() ? 1 : 0);
  });

  mainWindow.on('closed', () => {
    mainWindow = null
  });
}

app.on('ready', createWindow)

app.on('window-all-closed', () => {
  app.quit()
})
