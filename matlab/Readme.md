# Scripts de Modelado de un motor DC

En esta carpeta solo se encuentran los scripts que han sido creados o modificados por nosotros para hacer la labor de procesar los datos mas sencilla, o para adecuar los scripts proporcionados a las características de nuestro motor.

- **split.sh**: Separa los datos que se capturan con el test automático (en un solo fichero) en un fichero para cada experimento: desde `1V 1` hasta `12V 10`.
- **auto.m**: Usa los ficheros creados por el script anterior para generar los archivos `.mean` requeridos por el resto de funciones, haciendo la media de las repeticiones.
- **ModeladoMotorDCVelAngPoloBucleB**: Se ha modificado el número de experimentos y los pulsos por vuelta de nuestro motor.

El resto de archivos necesarios se encuentran en la [web del laboratorio](http://robolabo.etsit.upm.es/subjects.php?subj=seco&tab=tab2&lang=es)
