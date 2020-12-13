# Playlist Rank Viewer  
PlaylistRankViewer plugin for BakkesMod (https://bakkesplugins.com/plugins/view/125)

![Picture of ranks](https://i.imgur.com/uF8nsLN.png)

### About  
This plugin allows you to see the rank, MMR, and # of matches played by playlist for each player in the current session. 

### Usage  
Make sure that the plugin is enabled. Press F2 to open up the bakkes mod settings. Navigate to the settings tab, and click the check box for enabled under "PlaylistRankViewer".  Open the settings in BakkseMod to turn on/off data. Use the sliders to adjust size and location of the plugin on screen.

### Keyboard shortcuts
- `LeftShift + N` toggles the plugin
- `N` skips to the next player in the server

#### Tier Highlight Colors
- Unranked - White
- Bronze - Bronze/orange
- Silver - Muted gray
- Platinum - Light blue
- Diamond - Dark blue
- Champ - Purple
- Grand Champ - Red
- Supersonic Legend - Green  

Eample showing each tier highlight color:  
![Picture of ranks](https://i.imgur.com/q0ZbMqH.png)

### Manual Installation
- Download the `x64/Release/PlaylistRankViewerPlugin.dll` and `plugins/settings/playlistrankviewerplugin.set` files
- Open the Bakkesmod Injector and click File -> Open Bakkesmod folder
- Copy `PlaylistRankViewerPlugin.dll` into the plugins folder and `playlistrankviewerplugin.set` into the` plugins/settings` folder
- Open the `cfg/plugins.cfg` file and add a new line to it: `plugin load playlistrankviewerplugin` and save it Or watch Rocket Science's great video on how to install plugins
