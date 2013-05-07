SET (media_FILES 
    media/quafe.png
)

SET (glade_FILES
    ui/applicationwindow.glade
    ui/quafe-character.glade
    ui/pluginbutton.css
    ui/preferences.glade
)

INSTALL (FILES ${media_FILES}
    DESTINATION share/quafe-etk/media
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
    
INSTALL (FILES ${glade_FILES}
    DESTINATION share/quafe-etk/ui
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
                 