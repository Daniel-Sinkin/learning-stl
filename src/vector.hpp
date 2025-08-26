#include <new>
#include <format>
#include <stdexcept>
#include <algorithm>

namespace DS {
namespace Vector {
class Vector {
public:
	Vector(size_t n) {
		start_ = static_cast<int*>(::operator new[](n * sizeof(int)));
		end_ = start_;
		capacity_ = start_ + n;
	}

	~Vector() {
		::operator delete[](start_);
	}

	Vector(const Vector& other) {
		start_ = static_cast<int*>(::operator new[](other.get_capacity() * sizeof(int)));
		end_ = start_ + other.get_size();
		capacity_ = start_ + other.get_capacity();
	
		for(size_t i = 0; i < other.get_size(); ++i) start_[i] = other.start_[i];
	}

	void swap(Vector& other) noexcept {
		std::swap(start_, other.start_);
		std::swap(end_, other.end_);
		std::swap(capacity_, other.capacity_);
	}

	Vector& operator=(const Vector& other) {
		if(this != &other) {
			Vector tmp(other);
			swap(tmp);
		}
		return *this;
	}

	Vector(Vector&& other) noexcept
		: start_(other.start_), end_(other.end_), capacity_(other.capacity_)
	{
		other.start_ = nullptr;
		other.end_ = nullptr;
		other.capacity_ = nullptr;
	}

	Vector& operator=(Vector&& other) noexcept
	{
		if (this != &other) {
			::operator delete[](start_);

			start_ = other.start_;
			end_ = other.end_;
			capacity_ = other.capacity_;

			other.start_ = nullptr;
			other.end_ = nullptr;
			other.capacity_ = nullptr;
		}
		return *this;
	}

	// Returns true if we resized
	// Has amortized runtime of O(1)
	bool push_back(int value) {
		bool resized = false;
		if (end_ == capacity_) {
			size_t old_size = get_size();
			size_t old_cap = get_capacity();
			auto new_cap = old_cap ? old_cap * 2 : 1;

			int* new_start = static_cast<int*>(::operator new[](new_cap * sizeof(int)));

			for(size_t i = 0; i < old_size; ++i) new_start[i] = start_[i];

			::operator delete[](start_);
			start_ = new_start;
			end_ = start_ + old_size;
			capacity_ = start_ + new_cap;

			resized = true;
		}
		*end_ = value;
		++end_;
		return resized;
	}

	void pop() {
		if(is_empty()) throw std::runtime_error("pop_back on empty vector");
		--end_;
	}

	void reverse_inplace() { std::reverse(start_, end_); }
	
	void clear() noexcept { end_ = start_; }

	[[nodiscard("Use pop() if you don't need the return value.")]]
	int pop_back() {
		if(is_empty()) throw std::runtime_error("pop_back on empty vector");
		--end_;
		int value = *end_;
		return value;
	}

	[[nodiscard]] int get_last() const {
		if(is_empty()) throw std::runtime_error("get_last on empty vector");
		return end_[-1];
	}

	[[nodiscard]] int& operator[](size_t i) {return start_[i];}
	[[nodiscard]] const int& operator[](size_t i) const { return start_[i]; }
	[[nodiscard]] bool is_empty() const { return (start_ == end_); }
	[[nodiscard]] size_t get_size() const { return static_cast<std::size_t>(end_ - start_); }
	[[nodiscard]] size_t get_capacity() const { return static_cast<std::size_t>(capacity_ - start_); }
	[[nodiscard]] int* data() { return start_; }
	[[nodiscard]] const int* data() const { return start_; }
	[[nodiscard]] int* begin() { return start_; }
	[[nodiscard]] int* end() { return end_; }
	[[nodiscard]] const int* begin() const { return start_; }
	[[nodiscard]] const int* end() const { return end_; }
	[[nodiscard]] const int* cbegin() const { return start_; }
	[[nodiscard]] const int* cend() const { return end_; }
private:
	int* start_ = nullptr;
	int* end_ = nullptr;
	int* capacity_ = nullptr;
};

} // Vector
} // DS

template<>
struct std::formatter<DS::Vector::Vector> {
	bool show_capacity = false;

	constexpr auto parse(std::format_parse_context& ctx) {
		auto it = ctx.begin();
		auto end = ctx.end();
		if (it != end && *it == 'c') {
			show_capacity = true;
			++it;
		}
		if(it != end && *it != '}') throw std::format_error("invalid format");
		return it;
	}

	template<class FormatContext>
	auto format(const DS::Vector::Vector& v, FormatContext& ctx) const {
		auto out = ctx.out();
		out = std::format_to(out, "[");
		for(size_t i = 0; i < v.get_size(); ++i) {
			if(i) out = std::format_to(out, ", ");
			out = std::format_to(out, "{}", v.data()[i]);
		}
		out = std::format_to(out, "]");
		if(show_capacity) {
			out = std::format_to(out, " (size={}, cap={})", v.get_size(), v.get_capacity());
		}
		return out;
	}
};
