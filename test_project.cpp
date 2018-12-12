#include <string>
#include <deque>
#include <iostream>
#include <algorithm>

#include "test_runner.h"
using namespace std;

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() : pos(0) {}
  void Left() {
	  if(pos > 0) {
		  --pos;
	  }
  }
  void Right() {
	  if(pos < data.size()) {
		  ++pos;
	  }
  }
  void Insert(char token) {
	  data.insert(next(data.begin(), pos), token);
	  ++pos;
  }
  void Cut(size_t tokens = 1) {
	  size_t sz = CopyImpl(tokens);
	  data.erase(data.begin() + pos -1, data.begin() + pos -1 + sz);
  }
  void Copy(size_t tokens = 1) {
	  CopyImpl(tokens);
  }
  void Paste() {
	  data.insert(buf.begin(), buf.end(), data.begin() + pos -1);
  }
  string GetText() const { return string(data.begin(), data.end()); }
private:
  size_t CopyImpl(size_t tokens) {
	  if(tokens == 0) {
		  buf.clear();
		  return 0;
	  } else {
		  size_t sz = min(tokens, data.size() - pos +1);
		  buf.resize(sz);
		  copy(data.begin() + pos -1, data.begin() + pos -1 + sz, buf.begin());
		  return sz;
	  }
  }

  int pos;
  deque<char> data;
  deque<char> buf;
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestSimple() {
	Editor editor;
	TypeText(editor, "simple text");

	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Cut(3);
	editor.Right();
	editor.Paste();
	cout << editor.GetText();
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}
