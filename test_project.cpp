#include <string>
#include <deque>
#include <iostream>
#include <algorithm>
#include <list>

#include "test_runner.h"
using namespace std;

class Editor {
 public:
  Editor() {
	  pos = data.begin();
  }

  void Left() {
	  if(pos != data.begin()) {
		  pos = prev(pos);
	  }
  }

  void Right() {
	  if(pos != data.end()) {
		  pos = next(pos);
	  }
  }

  void Insert(char token) {
	  auto it = data.insert(pos, token);
	  pos = next(it);
  }

  void Cut(size_t tokens = 1) {
	  buf.clear();
      auto dist = distance(data.begin(), pos);
      size_t sz = min(tokens, data.size() - dist);
	  buf.splice(buf.begin(), data, pos, next(pos, sz));
      pos = next(data.begin(), dist);
  }

  void Copy(size_t tokens = 1) {
      buf.resize(tokens);
	  auto it = copy(pos, next(pos, tokens), buf.begin());
      buf.erase(it, buf.end());
  }

  void Paste() {
	  auto it = data.insert(pos, buf.begin(), buf.end());
	  pos = next(it, buf.size());
  }

  string GetText() const { return string(data.begin(), data.end()); }
private:
  list<char>::iterator pos;
  list<char> data;
  list<char> buf;
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
	editor.Copy();
	editor.Cut(3);
	editor.Right();
	editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "simple ttex");
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
