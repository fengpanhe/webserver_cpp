#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "locker.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "ThreadPool.h"

class http_conn : public ThreadClass{
public:
  static const int FILENAME_LEN = 200;       // 文件名的最大长度
  static const int READ_BUFFER_SIZE = 2048;  // 读缓存区的大小
  static const int WRITE_BUFFER_SIZE = 1024; // 写缓存区的大小
  // http的请求方法
  enum METHOD {
    GET = 0,
    POST,
    HEAD,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT,
    PATCH
  };
  // 解析用户请求时，主机状态机所处的状态
  enum CHECK_STATE {
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER,
    CHECK_STATE_CONTENT
  };
  // 服务器处理http请求的可能结果
  enum HTTP_CODE {
    NO_REQUEST,
    GET_REQUEST,
    BAD_REQUEST,
    NO_RESOURCE,
    FORBIDDEN_REQUEST,
    FILE_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION
  };
  // 行的读取状态
  enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };

public:
  http_conn() {}
  ~http_conn() {}
  // 初始化新接受的连接
  void init(int sockfd, const sockaddr_in &addr);
  // 关闭连接
  void close_conn(bool real_close = true);
  // 处理客户连接
  void run();
  // 非阻塞读
  bool read();
  // 非阻塞写
  bool write();

private:
  // 初始化连接
  void init();
  // 解析http请求
  HTTP_CODE process_read();
  // 填充http应答
  bool process_write(HTTP_CODE ret);
  // 以下一组函数被process_read 调用以分析 http 请求
  HTTP_CODE parse_request_line(char *text);
  HTTP_CODE parse_headers(char *text);
  HTTP_CODE parse_content(char *text);
  HTTP_CODE do_request();
  char *get_line() { return m_read_buf + m_start_line; }
  LINE_STATUS parse_line();
  // 以下一组函数被process_write 调用以填充 http 应答
  void unmap();
  bool add_response(const char *format, ...);
  bool add_content(const char *content);
  bool add_status_line(int status, const char *title);
  bool add_headers(int content_length);
  bool add_content_length(int content_length);
  bool add_linger();
  bool add_blank_line();

public:
  static int m_epollfd;
  static int m_user_count;

private:
  int m_sockfd;
  sockaddr_in m_address;
  // 读缓存区
  char m_read_buf[READ_BUFFER_SIZE];
  // 读缓存区下一个字节的位置
  int m_read_idx;
  // 当前正在分析的字符在读缓存区的位置
  int m_checked_idx;
  // 当前正在解析的行的起始位置
  int m_start_line;
  // 写缓存区
  char m_write_buf[WRITE_BUFFER_SIZE];
  // 写缓存区的字节数
  int m_write_idx;

  // 主机状态机当前所处的状态
  CHECK_STATE m_check_state;
  // 请求方法
  METHOD m_method;

  // 客户端请求的目标文件的完整路径
  char m_real_file[FILENAME_LEN];
  // 客户请求的目标文件的文件名
  char *m_url;
  // http协议的版本号
  char *m_version;
  // 主机名
  char *m_host;
  // http 请求的消息体长度
  int m_content_length;
  // http请求是否要求保持连接
  bool m_linger;

  // 客户请求的目标文件被mmap到内存中的起始位置
  char *m_file_address;
  // 目标文件的状态，通过此可以判断文件是否存在、是否为目录、是否可读，并获取文件大小等信息
  struct stat m_file_stat;
  // 使用writev来执行写操作，iv_count表示被写内存块的数量
  struct iovec m_iv[2];
  int m_iv_count;
};

#endif
