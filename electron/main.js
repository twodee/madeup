const {app, Menu, BrowserWindow, globalShortcut} = require('electron');
const path = require('path');
const url = require('url');
const LocalStorage = require('node-localstorage').LocalStorage;

let mainWindow;
let localStorage = new LocalStorage(require('os').homedir() + '/.madeup');

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
    height: height,
    icon: path.join(__dirname, 'assets/icons/png/1024x1024.png')
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

	const template = [
		{
			label: 'File',
			submenu: [
        {
          label: 'Open',
          accelerator: 'CommandOrControl+O',
          click() {
            mainWindow.webContents.send('open');
          },
        },
        {
          label: 'Save',
          accelerator: 'CommandOrControl+S',
          click() {
            mainWindow.webContents.send('save');
          },
        },
        {
          label: 'Save As...',
          accelerator: 'Shift+CommandOrControl+S',
          click() {
            mainWindow.webContents.send('saveAs');
          },
        },
			]
		},
		{
			label: 'Edit',
			submenu: [
				{role: 'undo'},
				{role: 'redo'},
				{type: 'separator'},
				{role: 'cut'},
				{role: 'copy'},
				{role: 'paste'},
				{role: 'pasteandmatchstyle'},
				{role: 'delete'},
				{role: 'selectall'}
			]
		},
		{
			label: 'View',
			submenu: [
				{role: 'reload'},
				{role: 'forcereload'},
				{role: 'toggledevtools'},
				{type: 'separator'},
				{role: 'resetzoom'},
				{role: 'zoomin'},
				{role: 'zoomout'},
				{type: 'separator'},
				{role: 'togglefullscreen'}
			]
		},
	];

  if (process.platform === 'darwin') {
    const name = app.getName();
    template.unshift({
      label: name,
      submenu: [
        {
          label: `About ${name}`,
          role: 'about',
        },
        { type: 'separator' },
        {
          label: 'Services',
          role: 'services',
          submenu: [],
        },
        { type: 'separator' },
        {
          label: `Hide ${name}`,
          accelerator: 'Command+H',
          role: 'hide',
        },
        {
          label: 'Hide Others',
          accelerator: 'Command+Alt+H',
          role: 'hideothers',
        },
        {
          label: 'Show All',
          role: 'unhide',
        },
        { type: 'separator' },
        {
          label: `Quit ${name}`,
          accelerator: 'Command+Q',
          click() { app.quit(); },
        },
      ],
    });
  }


	const menu = Menu.buildFromTemplate(template);
	Menu.setApplicationMenu(menu);
}

app.on('ready', createWindow)

app.on('window-all-closed', () => {
  app.quit()
})
