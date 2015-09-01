#include <vector>
#include <iostream>
#include <map>

namespace brainfuck {
struct B;

struct F {
  F() = delete;
  F(B* bptr) : x(bptr), bf(*this) {}
  B* x;
  F& bf;
  operator B();
  B operator++(int);
  B operator--(int);
  B operator[](const B& rhs);
  B operator++();
  B operator--();
  B operator+();
  B operator-();
  B operator+(const B& rhs);
  B operator-(const B& rhs);
  B operator<<(const B& rhs);
  B operator>>(const B& rhs);
  B operator<(const B& rhs);
  B operator>(const B& rhs);

};

struct B {
  B() : ignore(false), bf(this), offset(0) {}

  B(const B& to_copy) : ignore(false), prog(to_copy.prog), bf(this), braces(to_copy.braces), offset(to_copy.offset) {

    to_copy.ignore = true;
  }


  ~B(){
    if (ignore) return;

    reset_offset();


    std::vector<int> tape = {0};
    std::map<int, int> to_start, to_end;

    for (const auto& pair : braces) {
      to_end[pair.first] = pair.second;
      to_start[pair.second] = pair.first;
    }

    int prog_ptr = 0;
    int tape_ptr = 0;

    int prog_len = prog.size();
    while (prog_ptr < prog_len) {
      if (prog[prog_ptr] == '+') {
        tape[tape_ptr]++;

        prog_ptr++;
      } else if (prog[prog_ptr] == '-') {
        if (tape[tape_ptr] == 0) {
          throw "Cannot reduce cell below zero";
        }
        tape[tape_ptr]--;

        prog_ptr++;
      } else if (prog[prog_ptr] == '<') {
        if (tape_ptr == 0) {
          throw "Cannot move left of start";
        }
        tape_ptr--;

        prog_ptr++;
      } else if (prog[prog_ptr] == '>') {
        if (tape_ptr == static_cast<int>(tape.size()) - 1) {
          tape.push_back(0);
        }
        tape_ptr++;

        prog_ptr++;
      } else if (prog[prog_ptr] == '[') {
        if (tape[tape_ptr] == 0) {
          prog_ptr = to_end[prog_ptr];
        } else {
          prog_ptr++;
        }
      } else if (prog[prog_ptr] == ']') {
        if (tape[tape_ptr] != 0) {
          prog_ptr = to_start[prog_ptr];
        } else {
          prog_ptr++;
        }
      } else if (prog[prog_ptr] == '.') {
        std::cout << static_cast<char>(tape[tape_ptr]);
        prog_ptr++;
      }

    }
  }

  void reset_offset() const {
    for (auto& pair : braces) {
      pair.first += offset;
      pair.second += offset;
    }
    offset = 0;
  }

  void push_back(char c1, char c2 = 0) {
    prog.push_back(c1);
    if (c2 != 0) {
      prog.push_back(c2);
    }
  }


  B right_unary(char c1, char c2) {
    B output(*this);
    output.push_back(c1, c2);
    return output;
  }

  B operator[](const B& rhs) {
    B output(*this);
    rhs.ignore = true;
    output.push_back('[');
    output.prog.insert(output.prog.end(), rhs.prog.begin(), rhs.prog.end());
    output.push_back(']');
    output.braces.emplace_back(prog.size(), prog.size() + 1 + rhs.prog.size());
    return output;
  }

  B left_unary(char c1, char c2 = 0) {
    B output;
    ignore = true;
    output.push_back(c1, c2);
    output.prog.insert(output.prog.end(), prog.begin(), prog.end());
    output.braces.insert(output.braces.end(), braces.begin(), braces.end());
    output.offset = 1 + offset;
    if (c2 == 0) ++output.offset;

    return output;
  }

  B binary(const B& rhs, char c1, char c2 = 0) {
    reset_offset();
    B output(*this);
    rhs.ignore = true;
    output.push_back(c1, c2);

    rhs.offset += output.prog.size();
    rhs.reset_offset();

    output.prog.insert(output.prog.end(), rhs.prog.begin(), rhs.prog.end());
    output.braces.insert(output.braces.end(), rhs.braces.begin(), rhs.braces.end());

    return output;
  }

  B operator--(int) { return right_unary('-', '-'); }
  B operator++(int) { return right_unary('+', '+'); }
  B operator- () { return left_unary('-'); }
  B operator+ () { return left_unary('+'); }
  B operator--() { return left_unary('-', '-'); }
  B operator++() { return left_unary('+', '+'); }
  B operator+ (const B& rhs) { return binary(rhs, '+'); }
  B operator- (const B& rhs) { return binary(rhs, '-'); }
  B operator<<(const B& rhs) { return binary(rhs, '<', '<'); }
  B operator>>(const B& rhs) { return binary(rhs, '>', '>'); }
  B operator< (const B& rhs) { return binary(rhs, '<'); }
  B operator> (const B& rhs) { return binary(rhs, '>'); }


  mutable bool ignore;
  std::vector<char> prog;
  F bf;
  mutable std::vector<std::pair<int, int> > braces;
  mutable int offset;
} bf;

F::operator B() {
  x->ignore = true;
  B output(*x);
  output.push_back('.');
  return output;
}

B F::operator++(int) { return static_cast<B>(*this)++; }
B F::operator--(int) { return static_cast<B>(*this)--; }
B F::operator[](const B& rhs) { return static_cast<B>(*this)[rhs]; }
B F::operator++() { return ++static_cast<B>(*this); }
B F::operator--() { return --static_cast<B>(*this); }
B F::operator+() { return +static_cast<B>(*this); }
B F::operator-() { return -static_cast<B>(*this); }
B F::operator+(const B& rhs) { return static_cast<B>(*this) + rhs; }
B F::operator-(const B& rhs) { return static_cast<B>(*this) - rhs; }
B F::operator<<(const B& rhs) { return static_cast<B>(*this) << rhs; }
B F::operator>>(const B& rhs) { return static_cast<B>(*this) >> rhs; }
B F::operator<(const B& rhs) { return static_cast<B>(*this) < rhs; }
B F::operator>(const B& rhs) { return static_cast<B>(*this) > rhs; }
}
brainfuck::B& bf = brainfuck::bf;
