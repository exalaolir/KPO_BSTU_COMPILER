# Курсовой проект по КПО(БГТУ)
### Реализованно:
* беззнаковый целый тип(представлен как uint)
  * беззнаковый целый литерал начинается с 0u(пример: 0u55 эквивалентен 55)
* целый тип(представлен как int)
  * литерал пишется как обычный целочисленный(только десятичное представление)
* логический тип(представлен как bool)
    * литералы **true** и **false**. Тип bool поддерживает присваивание только этих литералов. Выражения *bool example = 5 > 3* неподеррживаются
* символьный тип(представлен как char)
    * литерал записывается в форме *'<символ>'*
* Число с плавающей точкой двойной точности(представлено как double)
    * Литерал полностью аналогичен тому, что в c++
* Строковой тип(Представлен как string)
    * Литерал пишется как *"<строка>"*. Сиволы \n, \v и \t необходимо экранировать(пример: *"Строка 1 /\n/ новая строка"*)
* Конструкции *if*, *if..else*, *while*
* Арифметические операторы *+*, *-*, *, */*, *%*
* Операторы сдвига *<<*, *>>*
* Условные операторы *<*, *>*, *<=*, *>=*, *:*(эквивалентно *==*), *:!*(эквивалентно *:!*)
* Автотипизация(представлена как let). По своему действию аналогична auto и c++ или var из c#
### Заключение
Этот репозиторий предназначен только для страдальцев, которым предстоит писать данную курсовою работу. Здесь много откровенно ужасного кода и хоть, по моему мнению, он тут намного понятнее, читабельнее и надёжнее примеров из тех же лаб, это не отменяет возможных ошибок. Так что используйте на свой страх и риск 
