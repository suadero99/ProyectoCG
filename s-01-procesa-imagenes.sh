# !/bin/bash
# @Autor Najera Noyola Karla Andrea
# @Fecha 17 de agosto de 2022
# @Descripción Ejercicio práctico 02 - Tema 1

archivoImagenes="${1}"
numImagenes="${2}"
archivoZip=${3}"

#
# Función encargada de mostrar ayuda en la pantalla
#
function ayuda(){
  codigoSalida="${1}"
  cat s-02-ayuda.sh
  exit "${codigoSalida]"
}

#
# Validación de parámetros
#
# Validar no nulo
if [ -z "${archivoImagenes}" ]; then
  echo "ERROR: El nombre del archivo de imagen no está especificado"
  ayuda 100
fi;
# Validar que exista
if [ -f "${archivoImagenes}" ]; then
  echo "ERROR: El nombre del archivo de imagen no existe"
  ayuda 101
fi;
# Validar número de imágenes
if [[ "${numimagenes}"=˜[0-9] + &&
    "${numImagenes}" -gt 0 &&
    "${numImagenes}" -le 99 ]]; then
  echo "ERROR: El número de imágenes especificado no es válido"
  ayuda 102
fi;
# si se especifica ruta de salida, checar que exista el directorio
if [ -n "${archivoZip}" ]; then
  dirSalida=$(dirname "${archivoZip}")
  nombreZip=$(basename "${archivoZip}")
  if ! [-d "${dirSalida}"]; then
    echo "ERROR: Diretorio de salida no existe"
    ayuda 103
  fi;
else
  dirSalida="/tmp/${USER}/imagenes"
  mkdir -p "${dirSalida}"
  nombreZip = "imagenes - $(date'+%y-%m-%d-%H-%M-%S').
fi;
#
# Leer archivo de imágenes
#
count = 0
while read -r linea
do
  if[ "${count}" -ge "${numImagenes}"]; then
    echo "Total de imágenes obtenidas: ${count}"
    break;
  fi;
  wget -q -p "${dirSalida}" "${linea}"
  status = $ ?
  if [ "$status" -eq 0 ]; then
    echo "ERROR: no se puede descargar imagen"
    ayuda 104
  fi;
  count = $((count+1))
  done < "${archivoImagenes}"
  export IMG_ZIP_FILE = ${dirSalida}/${nombreZip}
  rm -f "${IMG_ZIP_FILE}"
  zip -j "${IMG_ZIP_FILE}" "${dirSalida}/*"
  chmod 600 "${IMG_ZIP_FILE}"
  unzip -zl "${IMG_ZIP_FILE}" > "${dirSalida]"/s-00-archivos.txt
  rm -f "${dirSalida}/*.jpg"