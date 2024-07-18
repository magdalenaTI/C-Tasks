Напишете програма на C, която приема един задължителен позиционен параметър -
име на файл. Файлът се състои от не повече от 8 наредени тройки елементи:
• име на файл – точно 8 байта, последният от които задължително е 0 x 0 0 . Ако името е по-късо от
7 знака, излишните байтове са 0 x 0 0 ;
• offset – u i n t 3 2 _ t , който дава пореден номер на елемент (спрямо 𝑁0) във файла;
• length – u i n t 3 2 _ t , който дава брой елементи.
За всяка наредена тройка програмата трябва да пусне child процес, който да XOR-не (обработи с изключващо-
или) елементите (u i n t 1 6 _ t ) от съответния файл един със друг, и да върне резултата на parent процеса,
който от своя страна трябва да XOR-не всички получените резултати и да изведе полученото число в
следния формат:
r e s u l t : 5 7 3 B