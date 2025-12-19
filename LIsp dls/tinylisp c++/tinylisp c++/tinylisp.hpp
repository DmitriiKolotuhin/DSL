#ifndef TINYLISP_HPP
#define TINYLISP_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class TinyLisp {
public:
    // Конструктор
    TinyLisp(size_t memory_size = 1024 * 1024);

    // Вычисление выражений
    std::string eval(const std::string& code);

    // REPL
    void repl();

private:
    // --- Типы данных ---
    using Value = float;
    using Index = uint32_t;

    static constexpr Index ATOM_TAG = 0x7fc;
    static constexpr Index PRIM_TAG = 0x7fd;
    static constexpr Index CONS_TAG = 0x7fe;
    static constexpr Index CLOS_TAG = 0x7ff;
    static constexpr Index NIL_TAG = 0xfff;

    // --- Память ---
    std::vector<Value> memory;
    std::vector<char> string_heap;
    Index sp;
    Index hp;

    // Состояние чата (Внешнее хранилище для DSL)
    std::unordered_map<Index, Value> chat_state;

    // Специальные значения
    Value nil_value;
    Value true_value;
    Value error_value;
    Value env;

    // --- Парсер ---
    std::string input_buffer;
    size_t input_pos;
    char lookahead;
    char token_buffer[256]; // Увеличил буфер для длинных сообщений

    struct Primitive {
        const char* name;
        Value(TinyLisp::* func)(Value, Value);
    };
    std::vector<Primitive> primitives;

    // --- Helper functions ---
    Value box(Index tag, Index index) const;
    Index get_tag(Value val) const;
    Index get_index(Value val) const;
    Index get_type_bits(Value val) const;

    Value num(float n);
    bool equ(Value x, Value y);
    bool not_val(Value x);
    Value atom(const char* s);
    Value cons(Value x, Value y);
    Value car(Value p);
    Value cdr(Value p);
    Value pair(Value v, Value x, Value e);
    Value closure(Value v, Value x, Value e);
    Value assoc(Value v, Value e);

    // --- Core logic ---
    Value eval_expr(Value x, Value e);
    Value evlis(Value t, Value e);
    Value apply(Value f, Value t, Value e);
    Value bind(Value v, Value t, Value e);
    Value reduce(Value f, Value t, Value e);

    // --- Primitives ---
    Value p_eval(Value t, Value e);
    Value p_quote(Value t, Value e);
    Value p_cons(Value t, Value e);
    Value p_car(Value t, Value e);
    Value p_cdr(Value t, Value e);
    Value p_add(Value t, Value e);
    Value p_sub(Value t, Value e);
    Value p_mul(Value t, Value e);
    Value p_div(Value t, Value e);
    Value p_int(Value t, Value e);
    Value p_lt(Value t, Value e);
    Value p_eq(Value t, Value e);
    Value p_pair(Value t, Value e);
    Value p_or(Value t, Value e);
    Value p_and(Value t, Value e);
    Value p_not(Value t, Value e);
    Value p_cond(Value t, Value e);
    Value p_if(Value t, Value e);
    Value p_leta(Value t, Value e);
    Value p_lambda(Value t, Value e);
    Value p_define(Value t, Value e);

    // --- DSL Primitives ---
    Value p_msg_send(Value t, Value e);
    Value p_msg_get(Value t, Value e);
    Value p_msg_clear(Value t, Value e);
    Value p_msg_list_chats(Value t, Value e);

    // --- Parser & Print ---
    void setup_input(const std::string& code);
    void get_char();
    void skip_whitespace();
    char scan();
    Value read_one();
    Value parse_list();
    Value parse_quote();
    Value parse_atomic();

    std::string print(Value x);
    std::string print_list(Value t);
};

#endif