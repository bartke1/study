// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Utility functions and classes used by the Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)
//
// This file contains purely Google Test's internal implementation.  Please
// DO NOT #INCLUDE IT IN A USER PROGRAM.

#ifndef GTEST_SRC_GTEST_INTERNAL_INL_H_
#define GTEST_SRC_GTEST_INTERNAL_INL_H_

// GTEST_IMPLEMENTATION_ is defined to 1 iff the current translation unit is
// part of Google Test's implementation; otherwise it's undefined.
#if !GTEST_IMPLEMENTATION_
// A user is trying to include this from his code - just say no.
# error "gtest-internal-inl.h is part of Google Test's internal implementation."
# error "It must not be included except by Google Test itself."
#endif  // GTEST_IMPLEMENTATION_

#ifndef _WIN32_WCE
# include <errno.h>
#endif  // !_WIN32_WCE
#include <stddef.h>
#include <stdlib.h>  // For strtoll/_strtoul64/malloc/free.
#include <string.h>  // For memmove.

#include <algorithm>
#include <string>
#include <vector>

#include "gtest/internal/gtest-port.h"

#if GTEST_CAN_STREAM_RESULTS_
# include <arpa/inet.h>  // NOLINT
# include <netdb.h>  // NOLINT
#endif

#if GTEST_OS_WINDOWS
# include <windows.h>  // NOLINT
#endif  // GTEST_OS_WINDOWS

#include "gtest/gtest.h"  // NOLINT
#include "gtest/gtest-spi.h"

namespace testing {

// Declares the flags.
//
// We don't want the users to modify this flag in the code, but want
// Google Test's own unit tests to be able to access it. Therefore we
// declare it here as opposed to in gtest.h.
GTEST_DECLARE_bool_(death_test_use_fork);

namespace internal {

// The value of GetTestTypeId() as seen from within the Google Test
// library.  This is solely for testing GetTestTypeId().
GTEST_API_ extern const TypeId kTestTypeIdInGoogleTest;

// Names of the flags (needed for parsing Google Test flags).
const char kAlsoRunDisabledTestsFlag[] = "also_run_disabled_tests";
const char kBreakOnFailureFlag[] = "break_on_failure";
const char kCatchExceptionsFlag[] = "catch_exceptions";
const char kColorFlag[] = "color";
const char kFilterFlag[] = "filter";
const char kListTestsFlag[] = "list_tests";
const char kOutputFlag[] = "output";
const char kPrintTimeFlag[] = "print_time";
const char kRandomSeedFlag[] = "random_seed";
const char kRepeatFlag[] = "repeat";
const char kShuffleFlag[] = "shuffle";
const char kStackTraceDepthFlag[] = "stack_trace_depth";
const char kStreamResultToFlag[] = "stream_result_to";
const char kThrowOnFailureFlag[] = "throw_on_failure";

// A valid random seed must be in [1, kMaxRandomSeed].
const int kMaxRandomSeed = 99999;

// g_help_flag is true iff the --help flag or an equivalent form is
// specified on the command line.
GTEST_API_ extern bool g_help_flag;

// Returns the current time in milliseconds.
GTEST_API_ TimeInMillis GetTimeInMillis();

// Returns true iff Google Test should use colors in the output.
GTEST_API_ bool ShouldUseColor(bool stdout_is_tty);

// Formats the given time in milliseconds as seconds.
GTEST_API_ std::string FormatTimeInMillisAsSeconds(TimeInMillis ms);

// Converts the given time in milliseconds to a date string in the ISO 8601
// format, without the timezone information.  N.B.: due to the use the
// non-reentrant localtime() function, this function is not thread safe.  Do
// not use it in any code that can be called from multiple threads.
GTEST_API_ std::string FormatEpochTimeInMillisAsIso8601(TimeInMillis ms);

// Parses a string for an Int32 flag, in the form of "--flag=value".
//
// On success, stores the value of the flag in *value, and returns
// true.  On failure, returns false without changing *value.
GTEST_API_ bool ParseInt32Flag(
    const char* str, const char* flag, Int32* value);

// Returns a random seed in range [1, kMaxRandomSeed] based on the
// given --gtest_random_seed flag value.
inline int GetRandomSeedFromFlag(Int32 random_seed_flag) {
  const unsigned int raw_seed = (random_seed_flag == 0) ?
      static_cast<unsigned int>(GetTimeInMillis()) :
      static_cast<unsigned int>(random_seed_flag);

  // Normalizes the actual seed to range [1, kMaxRandomSeed] such that
  // it's easy to type.
  const int normalized_seed =
      static_cast<int>((raw_seed - 1U) %
                       static_cast<unsigned int>(kMaxRandomSeed)) + 1;
  return normalized_seed;
}

// Returns the first valid random seed after 'seed'.  The behavior is
// undefined if 'seed' is invalid.  The seed after kMaxRandomSeed is
// considered to be 1.
inline int GetNextRandomSeed(int seed) {
  GTEST_CHECK_(1 <= seed && seed <= kMaxRandomSeed)
      << "Invalid random seed " << seed << " - must be in [1, "
      << kMaxRandomSeed << "].";
  const int next_seed = seed + 1;
  return (next_seed > kMaxRandomSeed) ? 1 : next_seed;
}

// This class saves the values of all Google Test flags in its c'tor, and
// restores them in its d'tor.
class GTestFlagSaver {
 public:
  // The c'tor.
  GTestFlagSaver() {
    also_run_disabled_tests_ = GTEST_FLAG(also_run_disabled_tests);
    break_on_failure_ = GTEST_FLAG(break_on_failure);
    catch_exceptions_ = GTEST_FLAG(catch_exceptions);
    color_ = GTEST_FLAG(color);
    death_test_style_ = GTEST_FLAG(death_test_style);
    death_test_use_fork_ = GTEST_FLAG(death_test_use_fork);
    filter_ = GTEST_FLAG(filter);
    internal_run_death_test_ = GTEST_FLAG(internal_run_death_test);
    list_tests_ = GTEST_FLAG(list_tests);
    output_ = GTEST_FLAG(output);
    print_time_ = GTEST_FLAG(print_time);
    random_seed_ = GTEST_FLAG(random_seed);
    repeat_ = GTEST_FLAG(repeat);
    shuffle_ = GTEST_FLAG(shuffle);
    stack_trace_depth_ = GTEST_FLAG(stack_trace_depth);
    stream_result_to_ = GTEST_FLAG(stream_result_to);
    throw_on_failure_ = GTEST_FLAG(throw_on_failure);
  }

  // The d'tor is not virtual.  DO NOT INHERIT FROM THIS CLASS.
  ~GTestFlagSaver() {
    GTEST_FLAG(also_run_disabled_tests) = also_run_disabled_tests_;
    GTEST_FLAG(break_on_failure) = break_on_failure_;
    GTEST_FLAG(catch_exceptions) = catch_exceptions_;
    GTEST_FLAG(color) = color_;
    GTEST_FLAG(death_test_style) = death_test_style_;
    GTEST_FLAG(death_test_use_fork) = death_test_use_fork_;
    GTEST_FLAG(filter) = filter_;
    GTEST_FLAG(internal_run_death_test) = internal_run_death_test_;
    GTEST_FLAG(list_tests) = list_tests_;
    GTEST_FLAG(output) = output_;
    GTEST_FLAG(print_time) = print_time_;
    GTEST_FLAG(random_seed) = random_seed_;
    GTEST_FLAG(repeat) = repeat_;
    GTEST_FLAG(shuffle) = shuffle_;
    GTEST_FLAG(stack_trace_depth) = stack_trace_depth_;
    GTEST_FLAG(stream_result_to) = stream_result_to_;
    GTEST_FLAG(throw_on_failure) = throw_on_failure_;
  }

 public:
  // Fields for saving the original values of flags.
  bool also_run_disabled_tests_;
  bool break_on_failure_;
  bool catch_exceptions_;
  std::string color_;
  std::string death_test_style_;
  bool death_test_use_fork_;
  std::string filter_;
  std::string internal_run_death_test_;
  bool list_tests_;
  std::string output_;
  bool print_time_;
  internal::Int32 random_seed_;
  internal::Int32 repeat_;
  bool shuffle_;
  internal::Int32 stack_trace_depth_;
  std::string stream_result_to_;
  bool throw_on_failure_;
} GTEST_ATTRIBUTE_UNUSED_;

// Converts a Unicode code point to a narrow string in UTF-8 encoding.
// code_point parameter is of type UInt32 because wchar_t may not be
// wide enough to contain a code point.
// If the code_point is not a valid Unicode code point
// (i.e. outside of Unicode range U+0 to U+10FFFF) it will be converted
// to "(Invalid Unicode 0xXXXXXXXX)".
GTEST_API_ std::string CodePointToUtf8(UInt32 code_point);

// Converts a wide string to a narrow string in UTF-8 encoding.
// The wide string is assumed to have the following encoding:
//   UTF-16 if sizeof(wchar_t) == 2 (on Windows, Cygwin, Symbian OS)
//   UTF-32 if sizeof(wchar_t) == 4 (on Linux)
// Parameter str points to a null-terminated wide string.
// Parameter num_chars may additionally limit the number
// of wchar_t characters processed. -1 is used when the entire string
// should be processed.
// If the string contains code points that are not valid Unicode code points
// (i.e. outside of Unicode range U+0 to U+10FFFF) they will be output
// as '(Invalid Unicode 0xXXXXXXXX)'. If the string is in UTF16 encoding
// and contains invalid UTF-16 surrogate pairs, values in those pairs
// will be encoded as individual Unicode characters from Basic Normal Plane.
GTEST_API_ std::string WideStringToUtf8(const wchar_t* str, int num_chars);

// Reads the GTEST_SHARD_STATUS_FILE environment variable, and creates the file
// if the variable is present. If a file already exists at this location, this
// function will write over it. If the variable is present, but the file cannot
// be created, prints an error and exits.
void WriteToShardStatusFileIfNeeded();

// Checks whether sharding is enabled by examining the relevant
// environment variable values. If the variables are present,
// but inconsistent (e.g., shard_index >= total_shards), prints
// an error and exits. If in_subprocess_for_death_test, sharding is
// disabled because it must only be applied to the original test
// process. Otherwise, we could filter out death tests we intended to execute.
GTEST_API_ bool ShouldShard(const char* total_shards_str,
                            const char* shard_index_str,
                            bool in_subprocess_for_death_test);

// Parses the environment variable var as an Int32. If it is unset,
// returns default_val. If it is not an Int32, prints an error and
// and aborts.
GTEST_API_ Int32 Int32FromEnvOrDie(const char* env_var, Int32 default_val);

// Given the total number of shards, the shard index, and the test id,
// returns true iff the test should be run on this shard. The test id is
// some arbitrary but unique non-negative integer assigned to each test
// method. Assumes that 0 <= shard_index < total_shards.
GTEST_API_ bool ShouldRunTestOnShard(
    int total_shards, int shard_index, int test_id);

// STL container utilities.

// Returns the number of elements in the given container that satisfy
// the given predicate.
template <class Container, typename Predicate>
inline int CountIf(const Container& c, Predicate predicate) {
  // Implemented as an explicit loop since std::count_if() in libCstd on
  // Solaris has a non-standard signature.
  int count = 0;
  for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it) {
    if (predicate(*it))
      ++count;
  }
  return count;
}

// Applies a function/functor to each element in the container.
template <class Container, typename Functor>
void ForEach(const Container& c, Functor functor) {
  std::for_each(c.begin(), c.end(), functor);
}

// Returns the i-th element of the vector, or default_value if i is not
// in range [0, v.size()).
template <typename E>
inline E GetElementOr(const std::vector<E>& v, int i, E default_value) {
  return (i < 0 || i >= static_cast<int>(v.size())) ? default_value : v[i];
}

// Performs an in-place shuffle of a range of the vector's elements.
// 'begin' and 'end' are element indices as an STL-style range;
// i.e. [begin, end) are shuffled, where 'end' == size() means to
// shuffle to the end of the vector.
template <typename E>
void ShuffleRange(internal::Random* random, int begin, int end,
                  std::vector<E>* v) {
  const int size = static_cast<int>(v->size());
  GTEST_CHECK_(0 <= begin && begin <= size)
      << "Invalid shuffle range start " << begin << ": must be in range [0, "
      << size << "].";
  GTEST_CHECK_(begin <= end && end <= size)
      << "Invalid shuffle range finish " << end << ": must be in range ["
      << begin << ", " << size << "].";

  // Fisher-Yates shuffle, from
  // http://en.wikipedia.org/wiki/Fisher-Yates_shuffle
  for (int range_width = end - begin; range_width >= 2; range_width--) {
    const int last_in_range = begin + range_width - 1;
    const int selected = begin + random->Generate(range_width);
    std::swap((*v)[selected], (*v)[last_in_range]);
  }
}

// Performs an in-place shuffle of the vector's elements.
template <typename E>
inline void Shuffle(internal::Random* random, std::vector<E>* v) {
  ShuffleRange(random, 0, static_cast<int>(v->size()), v);
}

// A function for deleting an object.  Handy for being used as a
// functor.
template <typename T>
static void Delete(T* x) {
  delete x;
}

// A predicate that checks the key of a TestProperty against a known key.
//
// TestPropertyKeyIs is copyable.
class TestPropertyKeyIs {
 public:
  // Constructor.
  //
  // TestPropertyKeyIs has NO default constructor.
  explicit TestPropertyKeyIs(const std::string& key) : key_(key) {}

  // Returns true iff the test name of test property matches on key_.
  bool operator()(const TestProperty& test_property) const {
    return test_property.key() == key_;
  }

 public:
  std::string key_;
};

// Class UnitTestOptions.
//
// This class contains functions for processing options the user
// specifies when running the tests.  It has only static members.
//
// In most cases, the user can specify an option using either an
// environment variable or a command line flag.  E.g. you can set the
// test filter using either GTEST_FILTER or --gtest_filter.  If both
// the variable and the flag are present, the latter overrides the
// former.
class GTEST_API_ UnitTestOptions {
 public:
  // Functions for processing the gtest_output flag.

  // Returns the output format, or "" for normal printed output.
  static std::string GetOutputFormat();

  // Returns the absolute path of the requested output file, or the
  // default (test_detail.xml in the original working directory) if
  // none was explicitly specified.
  static std::string GetAbsolutePathToOutputFile();

  // Functions for processing the gtest_filter flag.

  // Returns true iff the wildcard pattern matches the string.  The
  // first ':' or '\0' character in pattern marks the end of it.
  //
  // This recursive algorithm isn't very efficient, but is clear and
  // works well enough for matching test names, which are short.
  static bool PatternMatchesString(const char *pattern, const char *str);

  // Returns true iff the user-specified filter matches the test case
  // name and the test name.
  static bool FilterMatchesTest(const std::string &test_case_name,
                                const std::string &test_name);

#if GTEST_OS_WINDOWS
  // Function for supporting the gtest_catch_exception flag.

  // Returns EXCEPTION_EXECUTE_HANDLER if Google Test should handle the
  // given SEH exception, or EXCEPTION_CONTINUE_SEARCH otherwise.
  // This function is useful as an __except condition.
  static int GTestShouldProcessSEH(DWORD exception_code);
#endif  // GTEST_OS_WINDOWS

  // Returns true if "name" matches the ':' separated list of glob-style
  // filters in "filter".
  static bool MatchesFilter(const std::string& name, const char* filter);
};

// Returns the current application's name, removing directory path if that
// is present.  Used by UnitTestOptions::GetOutputFile.
GTEST_API_ FilePath GetCurrentExecutableName();

// The role interface for getting the OS stack trace as a string.
class OsStackTraceGetterInterface {
 public:
  OsStackTraceGetterInterface() {}
  virtual ~OsStackTraceGetterInterface() {}

 public:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(OsStackTraceGetterInterface);
};

// A working implementation of the OsStackTraceGetterInterface interface.
class OsStackTraceGetter : public OsStackTraceGetterInterface {
 public:
  OsStackTraceGetter() : caller_frame_(NULL) {}

  // This string is inserted in place of stack frames that are part of
  // Google Test's implementation.
  static const char* const kElidedFramesMarker;

 public:

  // We save the stack frame below the frame that calls user code.
  // We do this because the address of the frame immediately below
  // the user code changes between the call to UponLeavingGTest()
  // and any calls to CurrentStackTrace() from within the user code.
  void* caller_frame_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(OsStackTraceGetter);
};

// Information about a Google Test trace point.
struct TraceInfo {
  const char* file;
  int line;
  std::string message;
};

// This is the default global test part result reporter used in UnitTestImpl.
// This class should only be used by UnitTestImpl.
class DefaultGlobalTestPartResultReporter
  : public TestPartResultReporterInterface {
 public:
  explicit DefaultGlobalTestPartResultReporter(UnitTestImpl* unit_test);
  // Implements the TestPartResultReporterInterface. Reports the test part
  // result in the current test.
  virtual void ReportTestPartResult(const TestPartResult& result);

 public:
  UnitTestImpl* const unit_test_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(DefaultGlobalTestPartResultReporter);
};

// The public implementation of the UnitTest class.  We don't protect
// the methods under a mutex, as this class is not accessible by a
// user and the UnitTest class that delegates work to this class does
// proper locking.
class GTEST_API_ UnitTestImpl {
 public:
  UnitTestImpl(UnitTest* parent)
  {}

  void AddTestInfo(TestInfo* test_info) {
      ti_ = test_info;
  }

  bool RunAllTests() {
      ti_->Run();
      return true;
  }

  TestInfo* ti_;
};  // class UnitTestImpl

// Convenience function for accessing the global UnitTest
// implementation object.
inline UnitTestImpl* GetUnitTestImpl() {
  return UnitTest::GetInstance()->impl();
}

#if GTEST_USES_SIMPLE_RE

// Internal helper functions for implementing the simple regular
// expression matcher.
GTEST_API_ bool IsInSet(char ch, const char* str);
GTEST_API_ bool IsAsciiDigit(char ch);
GTEST_API_ bool IsAsciiPunct(char ch);
GTEST_API_ bool IsRepeat(char ch);
GTEST_API_ bool IsAsciiWhiteSpace(char ch);
GTEST_API_ bool IsAsciiWordChar(char ch);
GTEST_API_ bool IsValidEscape(char ch);
GTEST_API_ bool AtomMatchesChar(bool escaped, char pattern, char ch);
GTEST_API_ bool ValidateRegex(const char* regex);
GTEST_API_ bool MatchRegexAtHead(const char* regex, const char* str);
GTEST_API_ bool MatchRepetitionAndRegexAtHead(
    bool escaped, char ch, char repeat, const char* regex, const char* str);
GTEST_API_ bool MatchRegexAnywhere(const char* regex, const char* str);

#endif  // GTEST_USES_SIMPLE_RE

// Parses the command line for Google Test flags, without initializing
// other parts of Google Test.
GTEST_API_ void ParseGoogleTestFlagsOnly(int* argc, char** argv);
GTEST_API_ void ParseGoogleTestFlagsOnly(int* argc, wchar_t** argv);

// TestResult contains some public methods that should be hidden from
// Google Test user but are required for testing. This class allow our tests
// to access them.
//
// This class is supplied only for the purpose of testing Google Test's own
// constructs. Do not use it in user tests, either directly or indirectly.
class TestResultAccessor {
 public:

  static void ClearTestPartResults(TestResult* test_result) {
    test_result->ClearTestPartResults();
  }

  static const std::vector<testing::TestPartResult>& test_part_results(
      const TestResult& test_result) {
    return test_result.test_part_results();
  }
};

#if GTEST_CAN_STREAM_RESULTS_

// Streams test results to the given port on the given host machine.
class StreamingListener : public EmptyTestEventListener {
 public:
  // Abstract base class for writing strings to a socket.
  class AbstractSocketWriter {
   public:
    virtual ~AbstractSocketWriter() {}

    // Sends a string to the socket.
    virtual void Send(const string& message) = 0;

    // Closes the socket.
    virtual void CloseConnection() {}

    // Sends a string and a newline to the socket.
    void SendLn(const string& message) {
      Send(message + "\n");
    }
  };

  // Concrete class for actually writing strings to a socket.
  class SocketWriter : public AbstractSocketWriter {
   public:
    SocketWriter(const string& host, const string& port)
        : sockfd_(-1), host_name_(host), port_num_(port) {
      MakeConnection();
    }

    virtual ~SocketWriter() {
      if (sockfd_ != -1)
        CloseConnection();
    }

    // Sends a string to the socket.
    virtual void Send(const string& message) {
      GTEST_CHECK_(sockfd_ != -1)
          << "Send() can be called only when there is a connection.";

      const int len = static_cast<int>(message.length());
      if (write(sockfd_, message.c_str(), len) != len) {
        GTEST_LOG_(WARNING)
            << "stream_result_to: failed to stream to "
            << host_name_ << ":" << port_num_;
      }
    }

   public:
    // Creates a client socket and connects to the server.
    void MakeConnection();

    // Closes the socket.
    void CloseConnection() {
      GTEST_CHECK_(sockfd_ != -1)
          << "CloseConnection() can be called only when there is a connection.";

      close(sockfd_);
      sockfd_ = -1;
    }

    int sockfd_;  // socket file descriptor
    const string host_name_;
    const string port_num_;

    GTEST_DISALLOW_COPY_AND_ASSIGN_(SocketWriter);
  };  // class SocketWriter

  // Escapes '=', '&', '%', and '\n' characters in str as "%xx".
  static string UrlEncode(const char* str);

  StreamingListener(const string& host, const string& port)
      : socket_writer_(new SocketWriter(host, port)) { Start(); }

  explicit StreamingListener(AbstractSocketWriter* socket_writer)
      : socket_writer_(socket_writer) { Start(); }

  void OnTestProgramStart(const UnitTest& /* unit_test */) {
    SendLn("event=TestProgramStart");
  }

  void OnTestProgramEnd(const UnitTest& unit_test) {
  }

  void OnTestCaseStart(const TestCase& test_case) {
    SendLn(std::string("event=TestCaseStart&name=") + test_case.name());
  }

  void OnTestCaseEnd(const TestCase& test_case) {
    SendLn("event=TestCaseEnd&passed=" + FormatBool(test_case.Passed())
           + "&elapsed_time=" + StreamableToString(test_case.elapsed_time())
           + "ms");
  }

  void OnTestStart(const TestInfo& test_info) {
    SendLn(std::string("event=TestStart&name=") + test_info.name());
  }

  void OnTestEnd(const TestInfo& test_info) {
    SendLn("event=TestEnd&passed=" +
           FormatBool((test_info.result())->Passed()) +
           "&elapsed_time=" +
           StreamableToString((test_info.result())->elapsed_time()) + "ms");
  }

  void OnTestPartResult(const TestPartResult& test_part_result) {
    const char* file_name = test_part_result.file_name();
    if (file_name == NULL)
      file_name = "";
    SendLn("event=TestPartResult&file=" + UrlEncode(file_name) +
           "&line=" + StreamableToString(test_part_result.line_number()) +
           "&message=" + UrlEncode(test_part_result.message()));
  }

 public:
  // Sends the given message and a newline to the socket.
  void SendLn(const string& message) { socket_writer_->SendLn(message); }

  // Called at the start of streaming to notify the receiver what
  // protocol we are using.
  void Start() { SendLn("gtest_streaming_protocol_version=1.0"); }

  string FormatBool(bool value) { return value ? "1" : "0"; }

  const scoped_ptr<AbstractSocketWriter> socket_writer_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(StreamingListener);
};  // class StreamingListener

#endif  // GTEST_CAN_STREAM_RESULTS_

}  // namespace internal
}  // namespace testing

#endif  // GTEST_SRC_GTEST_INTERNAL_INL_H_
