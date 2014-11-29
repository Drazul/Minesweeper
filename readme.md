Dependencias: 

- Ogre3D (como motor de renderizado)
- OIS (como gestor de eventos de teclado y ratón)

En los primeros commits de este repositorio se implementa la lógica del juego y se realiza con 
una aproximación de TDD, para ello se utiliza el framework de pruebas bandit, que a su vez 
utiliza el framework snowhouse.

https://github.com/joakimkarlsson/bandit

https://github.com/joakimkarlsson/snowhouse

Para que funcione se deben copiar las carpetas bandit y specs de bandit en la carpeta tests 
del proyecto. 
Tambien se debe copiar la carpeta snowhouse en test/bandit/assertion_frameworks/snowhouse
(dentro de este directorio va otro directorio llamado snowhouse)
