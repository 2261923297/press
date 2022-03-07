#pragma once

#include <memory>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <functional>


namespace tt {
namespace system {

class config_var_base {
public:
	config_var_base(const std::string& name
			, const std::string& desc = "")
		: m_name(name), m_desc(desc) {
		std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
	}

	const std::string& get_name() const { return m_name; }
	const std::string& get_desc() const { return m_name; }

	virtual bool toString(std::string& rt) = 0;
	virtual bool fromString(std::string& rt) = 0;
	
protected:
	std::string m_name;
	std::string m_desc;

}

template<class F, class T> 
class LexicalCast {
	public:
		T operator()(const F& f) {
			return (T)f;
		}

}; // class LexicalCast

template <class T
		 , class FromStr = LexicalCast<std::string, T>
		 , class ToStr = LexicalCast<T, std::string> >
class config_var : public config_var_base {
public:
	typedef std::function<void(const T& old, const T& new)> OnChangeCb_t;
	typedef std::shared_ptr<config_var> ptr;

	const T& get_val() const { return m_val; }
	void set_cb(OnChangeCb_t cb) { m_cbs} 

protected:
	T m_val;
	std::map<uint64_t, OnChangeCb_t>	m_cbs;

}; // class config_var

class config {
public:

	config();
	virtual ~config();

	int LookUp(const std::string& name,)


}; // class config
} // namespace system
} // namespace tt

