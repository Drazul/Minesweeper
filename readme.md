Dependencias: 

- Ogre3D (como motor de renderizado)
- OIS (como gestor de eventos de teclado y ratón)

Este repositorio utiliza el framework de pruebas bandit, que a su vez utiliza el framework snowhouse
(una vez probada la logica ya no tenia sentido seguir con los test y se han eliminado).


https://github.com/joakimkarlsson/bandit

https://github.com/joakimkarlsson/snowhouse

Para que funcione se deben copiar las carpetas bandit y specs de bandit en la carpeta tests del proyecto. 
Tambien se debe copiar la carpeta snowhouse en test/bandit/assertion_frameworks/snowhouse (dentro de este dir va otro dir llamado snowhouse)