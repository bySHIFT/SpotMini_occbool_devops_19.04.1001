# SpotMini_occbool_devops_19.04.1001

```
namespace internal
{
template<typename T>
class task;

template<typename R, typename... ArgTypes>
class task<R(ArgTypes...)>
{
public:
  task(std::function<R(ArgTypes...)>&& f) : m_fn(std::move(f)) {}
  task(std::function<R(ArgTypes...)>& f) : m_fn(f) {}

  template<typename... ArgTypes>
  R run(ArgTypes&&... args) { return m_fn(std::forward<ArgTypes>(args)...); }

  template<typename F>
  auto then(F&& f)/*->task<typename std::result_of<F(R)>::type(ArgTypes...)>*/
  {
    // (C++14 起)
    // (C++17 中弃用)
    // (C++20 中移除)
    using result_of_t = typename std::result_of<F(R)>::type;

    return task<result_of_t(ArgTypes...)>([this, &f](ArgTypes&&... args) {
      return f(m_fn(std::forward<ArgTypes>(args)...));
    });
  }

private:
  std::function<R(ArgTypes...)> m_fn;
};

class cicd
{
public:
  enum status
  {
    ERROR_S     = -1
    ,OK_S       = 0

    , CPPCHECK  = 1
    , BUILD     = (1 << 1)
    , CI        = (1 << 2)
    , CD        = (1 << 3)
    , CLEAN     = (1 << 4)

    , ALL       = (CPPCHECK | BUILD | CI | CD | CLEAN)
  };

  struct job
  {
    status stage;

    std::string get_stage_name(int step) const
    {
      switch (step)
      {
      case status::CPPCHECK:
        return std::string { "cppcheck" };
      case status::BUILD:
        return std::string { "build" };
      case status::CI:
        return std::string { "ci" };
      case status::CD:
        return std::string { "cd" };
      case status::CLEAN:
        return std::string { "clean" };
      }

      return std::string {};
    }

    friend std::ostream& operator<< (std::ostream& out, const job& jobs)
    {
      out << ">> job name: " << jobs.get_stage_name(jobs.stage) << std::endl
          << "    stage: " << jobs.stage << std::endl << std::endl;

      return out;
    }
  };
public:
  explicit cicd(const std::string& commit_id) : sha1(commit_id) {}
  explicit operator bool()
  {
    const auto over = jobs.stage - 1;
    return (over == steps);
  }

#define RUN_NEXT_JOB(step)    \
  jobs = job { step };        \
  if (!(*this)) return *this; \
  return run();

  cicd& cppcheck()  { RUN_NEXT_JOB(status::CPPCHECK); }
  cicd& build()     { RUN_NEXT_JOB(status::BUILD); }
  cicd& ci()        { RUN_NEXT_JOB(status::CI); }
  cicd& cd()        { RUN_NEXT_JOB(status::CD); }
  cicd& clean()     { RUN_NEXT_JOB(status::CLEAN); }

#undef RUN_NEXT_JOB

  int get_steps() const { return steps; }

private:
  const std::string sha1;
  job jobs;
  int steps { status::OK_S };

  cicd& run()
  {
    steps &= ~(1 << jobs.stage);
    steps |= jobs.stage;

    std::cout << sha1 << std::endl;
    std::cout << jobs;

    return *this;
  }
};

// 测试代码：
int TestTask()
{
  task <cicd&(cicd&)> my_cicd = \
    [](cicd& once_cicd) -> cicd& { return once_cicd; };
  cicd once_cicd("784dc082");
  const auto result = my_cicd
    .then([](cicd& commit){ return commit.cppcheck(); })
    .then([](cicd& commit){ return commit.build(); })
    .then([](cicd& commit){ return commit.ci(); })
    .then([](cicd& commit){ return commit.cd(); })
    .then([](cicd& commit){ return commit.clean(); })
    .run(once_cicd);

  const auto steps = result.get_steps();
  std::cout << std::endl
    << ">> STATUS: " << (steps == cicd::status::ALL ? "DONE" : "ERROR")
    << std::endl << std::endl;

  return (steps == cicd::status::ALL ? 0 : 1);
}
} // end namespace internal
```
