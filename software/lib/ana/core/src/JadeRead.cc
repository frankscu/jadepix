#include "JadeRead.hh"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FRAME_SIZE (4 + 48 * (4 + 16 * 2 + 4) + 4)

JadeRead::JadeRead(const JadeOption& opt)
    : m_opt(opt)
    , m_fd(0)
{
  SplitString(m_opt.GetStringValue("PATH"), m_path_vec, ";");
}

JadeRead::~JadeRead()
{
  Reset();
}

void JadeRead::Open()
{
  if (m_fd) {
    m_buf.clear();
#ifdef _WIN32
    _close(m_fd);
#else
    close(m_fd);
#endif
    m_fd = 0;
  }

  std::string path;

  if (!m_path_vec.empty()) {
    path = m_path_vec.back();
    std::cout << "JadeRead: file: " << path << std::endl;
    m_path_vec.pop_back();
  } else {
    std::cerr << "JadeRead: have readout all raw data files" << std::endl;
    throw;
  }

#ifdef _WIN32
  m_fd = _open(path.c_str(), _O_RDONLY | _O_BINARY);
#else
  m_fd = open(path.c_str(), O_RDONLY);
#endif
  if (m_fd <= 0) {
    std::cerr << "JadeRead: Failed to open devfile: " << path << "\n";
    m_fd = 0;
    throw;
  }
}

void JadeRead::Close()
{
  m_buf.clear();
  if (m_fd) {
#ifdef _WIN32
    _close(m_fd);
#else
    close(m_fd);
#endif
    m_fd = 0;
  }
}

void JadeRead::Reset()
{
  Close();
}

std::vector<JadeDataFrameSP>
JadeRead::Read(size_t nframe,
    const std::chrono::milliseconds& timeout)
{
  size_t size_buf = FRAME_SIZE * nframe;
  m_buf.resize(size_buf);
  std::vector<JadeDataFrameSP> v_df;
  v_df.reserve(nframe);

  size_t size_filled = 0;
  std::chrono::system_clock::time_point tp_timeout;
  bool can_time_out = false;
  uint32_t n = 0;
  uint32_t n_next = 4;
  while (size_filled < size_buf) {
#ifdef _WIN32
    if (n + 1 == n_next) {
      std::cout << n << "  " << size_filled << "  " << size_buf << "  " << (unsigned int)(size_buf - size_filled) << std::endl;
    }
    int read_r = _read(m_fd, &m_buf[size_filled], (unsigned int)(size_buf - size_filled));
    n++;
    if (n == n_next) {
      std::cout << n << std::endl;
      n_next = n_next * 2;
    }
#else
    int read_r = read(m_fd, &m_buf[size_filled], size_buf - size_filled);
#endif
    if (read_r < 0) {
      std::cerr << "JadeRead: reading error\n";
      throw;
    }

    if (read_r == 0) {
      std::cout << "read_r==0" << std::endl;
      return v_df;
    }
    size_filled += read_r;
    can_time_out = false;
  }
  size_t sub_beg = 0;
  while (sub_beg + FRAME_SIZE <= size_filled) {
    v_df.emplace_back(new JadeDataFrame(m_buf.substr(sub_beg, FRAME_SIZE)));
    sub_beg += FRAME_SIZE;
  }
  return v_df;
}

void JadeRead::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while (std::string::npos != pos2) {
    v.push_back(s.substr(pos1, pos2 - pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if (pos1 != s.length())
    v.push_back(s.substr(pos1));
}
