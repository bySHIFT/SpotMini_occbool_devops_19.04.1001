# SpotMini_occbool_devops_19.04.1001

```
namespace internal
{
template<typename T>
class task;

template<typename R, typename... Args>
class task<R(Args...)>
{
public:
  task(std::function<R(Args...)>&& f) : m_fn(std::move(f)) {}
  task(std::function<R(Args...)>& f) : m_fn(f) {}

  template<typename... Args>
  R run(Args&&... args) { return m_fn(std::forward<Args>(args)...); }

  template<typename F>
  auto then(F& f)/*->task<typename std::result_of<F(R)>::type(Args...)>*/
  {
    using return_type = typename result_of<F(R)>::type;
    return task<return_type(Args...)>([this, &f](Args&&... args) {
      return f(m_fn(std::forward<Args>(args)...));
    });
  }

private:
  std::function<R(Args...)> m_fn;
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

#define RUN_NEXT_JOB(next_cicd, step) \
  next_cicd.jobs = job { step };      \
  if (!next_cicd) return next_cicd;   \
  return cicd::run(next_cicd);

  cicd& cppcheck(cicd& next)  { RUN_NEXT_JOB(next, status::CPPCHECK); }
  cicd& build(cicd& next)     { RUN_NEXT_JOB(next, status::BUILD); }
  cicd& ci(cicd& next)        { RUN_NEXT_JOB(next, status::CI); }
  cicd& cd(cicd& next)        { RUN_NEXT_JOB(next, status::CD); }
  cicd& clean(cicd& next)     { RUN_NEXT_JOB(next, status::CLEAN); }

#undef RUN_NEXT_JOB

  int get_steps() const { return steps; }

private:
  const std::string sha1;
  job jobs;
  int steps { status::OK_S };

  static cicd& run(cicd& current_cicd)
  {
    current_cicd.steps &= ~(1 << current_cicd.jobs.stage);
    current_cicd.steps |= current_cicd.jobs.stage;

    std::cout << current_cicd.sha1 << std::endl;
    std::cout << current_cicd.jobs;

    return current_cicd;
  }
};

// 测试代码：
int TestTask()
{
  task <cicd&(cicd&)> my_cicd = \
    [](cicd& once_cicd) -> cicd& { return once_cicd; };
  cicd once_cicd("784dc082");
  const auto result = my_cicd
    .then([&once_cicd](cicd& commit){ return once_cicd.cppcheck(commit); })
    .then([&once_cicd](cicd& commit){ return once_cicd.build(commit); })
    .then([&once_cicd](cicd& commit){ return once_cicd.ci(commit); })
    .then([&once_cicd](cicd& commit){ return once_cicd.cd(commit); })
    .then([&once_cicd](cicd& commit){ return once_cicd.clean(commit); })
    .run(once_cicd);

  const auto steps = result.get_steps();
  std::cout << std::endl
    << ">> STATUS: " << (steps == cicd::status::ALL ? "DONE" : "ERROR")
    << std::endl << std::endl;

  return (steps == cicd::status::ALL ? 0 : 1);
}
} // end namespace internal
```
