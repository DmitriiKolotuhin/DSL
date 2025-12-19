# TinyLisp C++ Messenger DSL

> Встраиваемый DSL на базе Lisp для управления состоянием клиента мессенджера, реализованный на C++.

Этот проект демонстрирует разработку и практическое применение встраиваемого языка специфического домена (DSL) на базе собственного интерпретатора Lisp, написанного на C++. DSL позволяет управлять состоянием клиента мессенджера — создавать чаты, отправлять сообщения, получать список сообщений и очищать чаты — с помощью простых и выразительных конструкций, интегрированных в Lisp-синтаксис.

---

## Цель проекта

Разработать прототип встраиваемого DSL на базе Lisp для управления состоянием клиента мессенджера и продемонстрировать его работоспособность на примере простого клиентского приложения.

---

##  Особенности

- Реализован на чистом C++ без внешних зависимостей.
- Использует собственный интерпретатор Lisp (`TinyLisp`) с поддержкой атомов, списков, функций и примитивов.
- DSL предоставляет следующие функции:
  - `(msg-send! 'chat-name 'sender "message")` — отправить сообщение в чат.
  - `(msg-get 'chat-name)` — получить список сообщений из чата.
  - `(msg-clear! 'chat-name)` — очистить чат.
  - `(msg-list-chats)` — перечислить все активные чаты.
- Возможность определять пользовательские функции на Lisp, используя примитивы DSL (например, `(define broadcast ...)`).
- Состояние хранится во внешнем хранилище (`std::unordered_map`) и сохраняется между вызовами.

---

##  Быстрый старт

### Требования

- Компилятор C++17 (g++, clang++)
- ОС: Windows, Linux, macOS

### Установка и сборка

1. Клонируйте репозиторий:

   ```bash
   git clone https://github.com/ваш-логин/ваш-проект.git
   cd ваш-проект

2. После запуска программа автоматически выполнит тесты DSL:

Testing Messenger DSL functions:
DSL Test: (msg-send! 'general 'Alice "Hello everyone!")
Result: #t
------------------------
DSL Test: (msg-send! 'general 'Bob "Hi Alice")
Result: #t
------------------------
DSL Test: (msg-send! 'private 'Charlie "Secret message")
Result: #t
------------------------
DSL Test: (msg-list-chats)
Result: (private general)
------------------------
DSL Test: (msg-get 'general)
Result: ((Bob . "Hi Alice") (Alice . "Hello everyone!"))
------------------------
DSL Test: (msg-get 'private)
Result: ((Charlie . "Secret message"))
------------------------
DSL Test: (msg-clear! 'private)
Result: #t
------------------------
DSL Test: (msg-get 'private)
Result: ()
------------------------
DSL Test: (msg-list-chats)
Result: (general)
------------------------

3. Также запустится интерактивная оболочка (REPL), где можно выполнять произвольные выражения:

Starting REPL (enter expressions):
> (define broadcast(lambda (sender text)(list(msg-send! 'general sender text)(msg-send! 'news sender text))))
broadcast
> (broadcast 'Admin "Server restart in 5 min")
(#t #t)
> (define last-msg (car (msg-get 'general)))
last-msg
> (cdr last-msg)
"Server restart in 5 min"
