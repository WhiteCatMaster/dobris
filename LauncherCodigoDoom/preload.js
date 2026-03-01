const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
  lanzar: (carpeta, wad, ip, puerto, sistemaElegido) => {
    ipcRenderer.send('lanzar-comando', carpeta, wad, ip, puerto, sistemaElegido);
  }
});