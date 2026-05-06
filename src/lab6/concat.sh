#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo "Ошибка: неверное количество аргументов"
    echo "Использование: $0 <файл> <кол-во_повторов> <макс_размер_байт> <выходной_файл>"
    exit 1
fi

FILE=$1
REPS=$2
MAX_SIZE=$3
OUT=$4

if [ ! -f "$FILE" ]; then
    echo "Ошибка: файл не найден"
    exit 1
fi

FILE_SIZE=$(stat -c%s "$FILE")
if [ "$FILE_SIZE" -eq 0 ]; then
    echo "Ошибка: исходный файл пуст. Конкатенация бессмысленна"
    exit 1
fi

MAX_BY_SIZE=$(( MAX_SIZE / FILE_SIZE ))
if [ "$MAX_BY_SIZE" -lt "$REPS" ]; then
    COUNT=$MAX_BY_SIZE
else
    COUNT=$REPS
fi

> "$OUT"

for (( i=0; i<COUNT; i++ )); do
    cat "$FILE" >> "$OUT"
done

echo "Файл записан $COUNT раз(а). Итоговый размер: $(stat -c%s "$OUT") байт"