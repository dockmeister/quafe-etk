SET (media_FILES 
    media/quafe.png
)

SET (glade_FILES
    ui/applicationwindow.glade
)

INSTALL (FILES ${media_FILES} ${glade_FILES} ${plugin_FILES}
    DESTINATION share/quafe-etk/
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
    
    
                 