#include "tinylisp.hpp"
#include <iostream>
#include <vector>

int main() {
    // Выделяем достаточно памяти (1МБ), так как мы отключили GC
    TinyLisp lisp(1024 * 1024);

    std::cout << "=== TinyLisp C++ Messenger DSL ===\n" << std::endl;

    std::vector<std::string> tests = {
        "(define equal? (lambda (x y) (or (eq? x y) (and (pair? x) (pair? y) (equal? (car x) (car y)) (equal? (cdr x) (cdr y))))))",
        "(define list (lambda args args))",
        "(if (equal? ((lambda (l) (+ . l)) '(1 2 3)) 6) 'passed 'failed)",
        "(if (equal? ((lambda (l) (- . l)) '(1 2 3)) -4) 'passed 'failed)",
        "(if (equal? ((lambda (l) (* . l)) '(1 2 3)) 6) 'passed 'failed)",
        "(if (equal? (let* (x 1) (y (+ 1 x)) (let* (z (+ x y)) z)) 3) 'passed 'failed)",
        "(if (equal? (((lambda (f x) (lambda args (f x . args))) + 1) 2 3) 6) 'passed 'failed)",
        "(if (equal? ((lambda (l) ((lambda (x y z) (list x y z)) '(1) '(2) . l)) '((3))) '((1) (2) (3))) 'passed 'failed)"
    };

    std::cout << "Running standard tests...\n";
    for (const auto& test : tests) {
        try {
            std::string res = lisp.eval(test);
            if (res != "passed" && test.find("(define") == std::string::npos) {
                std::cout << "Test: " << test << "\nResult: " << res << "\n------------------------\n";
            }
            else if (test.find("(define") != std::string::npos) {
                std::cout << "Defined: " << res << "\n";
            }
            else {
                std::cout << "Test passed.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Runtime Error: " << e.what() << std::endl;
        }
    }

    std::cout << "\nTesting Messenger DSL functions:\n";

    std::vector<std::string> dsl_tests = {
        "(msg-send! 'general 'Alice \"Hello everyone!\")",
        "(msg-send! 'general 'Bob \"Hi Alice\")",
        "(msg-send! 'private 'Charlie \"Secret message\")",
        "(msg-list-chats)",
        "(msg-get 'general)",
        "(msg-get 'private)",
        "(msg-clear! 'private)",
        "(msg-get 'private)",
        "(msg-list-chats)"
    };

    for (const auto& test : dsl_tests) {
        std::cout << "DSL Test: " << test << "\nResult: ";
        try {
            std::cout << lisp.eval(test) << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Runtime Error: " << e.what() << std::endl;
        }
        std::cout << "------------------------\n";
    }

    std::cout << "\nStarting REPL (enter expressions):\n";
    lisp.repl();

    return 0;
}