#include <cstring>
#include <stdio.h>
using namespace std;
char *url_;
char *version_;
void parse_request_line(char *text) {
  printf("text = %s\n", text);
  url_ = strpbrk(text, " \t");
  printf("url_ = %s\n", url_);
  if (!url_) {
    printf("url_ = %s\n", url_);
  }
  *url_++ = '\0';
  printf("text = %s\n", text);
  char *method = text;
  if (strcasecmp(method, "GET") == 0) {
    // ;method_ = GET
    printf("method_ = GET%s\n", method);
  } else {
    printf("method_ != GET%s\n", method);
  }

  url_ += strspn(url_, " \t");
  version_ = strpbrk(url_, " \t");
  if (!version_) {
    printf("%s\n", version_);
  }
  *version_++ = '\0';
  version_ += strspn(version_, " \t");
  if (strcasecmp(version_, "HTTP/1.1") != 0) {
    printf("%s\n", version_);
  }

  if (strncasecmp(url_, "http://", 7) == 0) {
    url_ += 7;
    url_ = strchr(url_, '/');
  }

  if (!url_ || url_[0] != '/') {
    printf("%s\n", url_);
  }

  printf("%s\n", url_);

  //   check_state_ = CHECK_STATE_HEADER;
}
int main() {
  // char ch[100] = "GET /index.html HTTP/1.1";
  // char *a = strpbrk(ch, " \t");
  // *a++ = '\0';
  // printf("%s\n", a);
  // printf("%s\n", ch);
  char ch[10] = "as";
  printf("ch_len:%d\n", strlen(ch));
  int ch_len = strlen(ch);
  char *ab = new char[ch_len];
  strcpy(ab, ch);
  printf("%s %s\n", ch, ab);
  //   parse_request_line(ch);
}