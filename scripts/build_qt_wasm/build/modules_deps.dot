// taken from .gitmodules
digraph G {
  graph [rankdir=LR]

  // Essential
  { node [color="#aa0000" shape=doubleoctagon] 
    qtbase
    qtdeclarative
    qttools
    qttranslations
    qtdoc
    qtrepotools
    qtqa
  }

  // Addon
  { node [color="#00aa00" shape=hexagon]
    qtsvg 
    qtactiveqt
    qtmultimedia
    qtpositioning
    qtsensors
    qtconnectivity
    qtwayland
    qt3d
    qtimageformats
    qtserialbus
    qtserialport
    qtwebsockets
    qtwebchannel
    qtwebengine
    qtwebview
    qtcharts
    qtdatavis3d
    qtvirtualkeyboard
    qtscxml
    qtnetworkauth
    qtremoteobjects
    qtlottie
    qtquicktimeline
    qtquick3d
    qtshadertools
    qtcoap
    qtmqtt
    qtopcua
  } 

  // Ignore
  { node [color="#cccccc" shape=octagon]
    qtxmlpatterns
    qtlocation
    qtsystems
    qtfeedback
    qtpim
    qtcanvas3d
    qtgamepad
    qtspeech
    qtwebglplugin
  } 

  // Deprecated
  { node [color="#FF0000" shape=box] qt5compat }

  // Preview
  { node [color="#0000FF" shape=box] qtlanguageserver }

  // Depends
  { edge []
	  qtbase                -> qtsvg
    qtbase                -> qtdeclarative
    qtbase                -> qtactiveqt
    qtbase,qtshadertools  -> qtmultimedia
    qtbase                -> qttools
    qtbase                -> qtxmlpatterns
    qttools               -> qttranslations
    qtdeclarative,qttools -> qtdoc
    qtbase                -> qtqa
    qtbase,qtpositioning  -> qtlocation
    qtbase                -> qtpositioning
    qtbase                -> qtsensors
    qtbase                -> qtsystems
    qtdeclarative         -> qtfeedback
    qtdeclarative         -> qtpim
    qtbase                -> qtconnectivity
    qtbase                -> qtwayland
    qtbase                -> qt3d
    qtbase                -> qtimageformats
    qtbase                -> qtserialbus
    qtbase                -> qtserialport
    qtbase                -> qtwebsockets
    qtbase                -> qtwebchannel
    qtdeclarative         -> qtwebengine
    qtdeclarative         -> qtcanvas3d
    qtdeclarative         -> qtwebview
    qtbase                -> qtcharts
    qtbase                -> qtdatavis3d
    qtbase,qtdeclarative,qtsvg -> qtvirtualkeyboard
    qtbase               -> qtgamepad
    qtbase,qtdeclarative -> qtscxml
    qtbase               -> qtspeech
    qtbase               -> qtnetworkauth
    qtbase               -> qtremoteobjects
    qtbase,qtwebsockets  -> qtwebglplugin
    qtbase,qtdeclarative -> qtlottie
    qtbase,qtdeclarative -> qtquicktimeline
    qtbase,qtdeclarative,qtshadertools -> qtquick3d
    qtbase               -> qtshadertools
    qtbase,qtdeclarative -> qt5compat
    qtbase -> qtcoap
    qtbase,qtdeclarative -> qtmqtt
    qtbase,qtdeclarative -> qtopcua
    qtbase -> qtlanguageserver
  }

  // Recommends
  { edge [color="#0000ee"]
	  qtimageformats,qtshadertools,qtsvg,qtlanguageserver -> qtdeclarative
    qtdeclarative               -> qtmultimedia
    qtdeclarative,qtactiveqt    -> qttools
    qtdeclarative               -> qtxmlpatterns
    qtmultimedia                -> qtdoc
    qtdeclarative               -> qtlocation
    qtdeclarative,qtserialport  -> qtpositioning
    qtdeclarative               -> qtsensors
    qtdeclarative               -> qtsystems
    qtmultimedia                -> qtfeedback
    qtdeclarative               -> qtconnectivity
    qtdeclarative               -> qtwayland
    qtdeclarative,qtshadertools -> qt3d
    qtserialport                -> qtserialbus
    qtdeclarative               -> qtwebsockets
    qtdeclarative,qtwebsockets  -> qtwebchannel
    qtwebchannel,qttools,qtpositioning -> qtwebengine
    qtwebengine                 -> qtwebview
    qtdeclarative,qtmultimedia  -> qtcharts
    qtdeclarative,qtmultimedia  -> qtdatavis3d
    qtmultimedia                -> qtvirtualkeyboard
    qtdeclarative               -> qtgamepad
    qtdeclarative,qtmultimedia  -> qtspeech
    qtdeclarative               -> qtremoteobjects
    qtdeclarative               -> qtwebglplugin
    qtquicktimeline             -> qtquick3d
  }
}
