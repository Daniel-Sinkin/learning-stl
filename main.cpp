#include <new>
#include <format>
#include <print>

using std::print, std::println;

class Vector {
public:
	Vector(int n) {
		// `::operator new` throws std::bad_alloc, no need to check for failure
		start_ = static_cast<int*>(::operator new[](n * sizeof(int)));
		end_ = start_;
		capacity_ = start_ + n;
	}
	~Vector() {
		::operator delete[](start_);
	}

	// Returns true if we resized
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

	bool is_empty() const { return (start_ == end_); }

	size_t get_size() const { return static_cast<std::size_t>(end_ - start_); }
	size_t get_capacity() const { return static_cast<std::size_t>(capacity_ - start_); }
	const int* data() const { return start_; }
private:
	int* start_ = nullptr;
	int* end_ = nullptr;
	int* capacity_ = nullptr;
};

template<>
struct std::formatter<Vector> {
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
	auto format(const Vector& v, FormatContext& ctx) const {
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

void Vector__push_back_verbose(Vector& vector, int value) {
	println("Invoked Vector__push_back_verbose:");
	println("\tPushing value={} into {:c}", value, vector);
	if(vector.push_back(value)) {
		println("\tTriggered Resize!");
	}
	println("\tResulting vector: {:c}", vector);
}

int main() {
	Vector vec(2);

	for(size_t i = 0; i < 8; ++i) Vector__push_back_verbose(vec, static_cast<int>(i));
}
