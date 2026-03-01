const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const { exec, spawn } = require('child_process');
let ventana; 

function crearVentana () {
  ventana = new BrowserWindow({
    width: 450,
    height: 676, 
    resizable: true,
    icon: path.join(__dirname, 'DoomlauncherFav.png'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  });

  ventana.loadFile('index.html');
  ventana.setMenuBarVisibility(false);
}

app.whenReady().then(() => {
  crearVentana();

  ipcMain.on('lanzar-comando', (event, carpeta, wad, ip, puerto, sistemaElegido, receiver) => {
    
    //WINDOWS
    if (sistemaElegido === 'windows') {
      const comando = `chocolate-doom.exe -iwad "${wad}" -connect ${ip} -port ${puerto}`;
      exec(comando, { cwd: carpeta }, (error) => {
        if (error) event.reply('mensaje-receiver', `<span style="color:red">Error Windows: ${error.message}</span>`);
      });
     
    //LINUX
    } else {
      const procesoDoom = spawn('chocolate-doom', ['-iwad', wad, '-connect', ip, '-port', puerto.toString()]);

      // Para cuando hayan errores
      procesoDoom.stderr.on('data', (data) => {
        event.reply('mensaje-receiver', `<span style="color:orange">DOOM: ${data.toString()}</span>`);
      });

      procesoDoom.on('error', (error) => {
        event.reply('mensaje-receiver', `<span style="color:red">Error al ejecutar chocolate-doom: ${error.message}</span>`);
      });

      if (receiver && receiver.trim() !== "") {
        const procesoReceiver = spawn(receiver, [], { shell: true });

        procesoReceiver.stdout.on('data', (data) => {
          event.reply('mensaje-receiver', data.toString());
        });

        procesoReceiver.stderr.on('data', (data) => {
          event.reply('mensaje-receiver', `<span style="color:red">Error: ${data.toString()}</span>`);
        });

        procesoReceiver.on('close', (code) => {
          event.reply('mensaje-receiver', `<span style="color:gray">--- Proceso terminado (Código ${code}) ---</span>`);
        });
        
        procesoReceiver.on('error', (err) => {
            event.reply('mensaje-receiver', `<span style="color:red">Error al ejecutar Receiver: ${err.message}</span>`);
        });
      }
    }
  });
});

app.on('window-all-closed', () => {
  app.quit();
});

ventana

