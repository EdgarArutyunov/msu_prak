char * read_word();
/*
    Игнорирует первые пробельные символы.
    Дальше начинает считываение в четырех режимах.
    0. Ожидание одинарной скобки
    1. Ожидание парной
    2. Ожидание любого символа
    3. Свободный.
    Возвращает очередной аргумент argv[..] при запуске программы.
*/

plist parse_cmd(int * wasEOF);