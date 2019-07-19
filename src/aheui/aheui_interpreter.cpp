#include <talkheui/aheui/interpreter.hpp>

#include <talkheui/aheui/extension.hpp>
#include <talkheui/aheui/storage.hpp>
#include <talkheui/encoding.hpp>
#include <talkheui/hangul.hpp>

#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <u5e/basic_grapheme.hpp>

namespace talkheui::aheui
{
	runtime_state::runtime_state()
	{
		reset();
	}

	void runtime_state::reset()
	{
		memories().clear();

		for (int i = 0; i < 27; ++i)
		{
			memories().push_back(new stack());
		}

		memories().insert(memories().begin() + 21, new queue());

		reset_step();
	}
	void runtime_state::reset_step()
	{
		x_ = 0;
		y_ = -1;
		dx_ = 0;
		dy_ = 1;
	}

	int runtime_state::x() const noexcept
	{
		return x_;
	}
	void runtime_state::x(int new_x) noexcept
	{
		x_ = new_x;
	}
	int runtime_state::y() const noexcept
	{
		return y_;
	}
	void runtime_state::y(int new_y) noexcept
	{
		y_ = new_y;
	}
	int runtime_state::dx() const noexcept
	{
		return dx_;
	}
	void runtime_state::dx(int new_dx) noexcept
	{
		dx_ = new_dx;
	}
	int runtime_state::dy() const noexcept
	{
		return dy_;
	}
	void runtime_state::dy(int new_dy) noexcept
	{
		dy_ = new_dy;
	}
	aheui::extension* runtime_state::extension() const noexcept
	{
		return extension_;
	}
	void runtime_state::extension(aheui::extension* new_extension)
	{
		delete memories().back();

		if (new_extension)
		{
			memories().back() = new pipe(new_extension);
		}
		else
		{
			memories().back() = new stack();
		}

		extension_ = new_extension;
	}
	std::size_t runtime_state::selected_memory() const noexcept
	{
		return selected_memory_;
	}
	void runtime_state::selected_memory(std::size_t new_selected_memory) noexcept
	{
		selected_memory_ = new_selected_memory;
	}
}

namespace talkheui::aheui
{
	interpreter::interpreter() noexcept
		: talkheui::interpreter("Aheui")
	{
		state(new runtime_state());
	}
	interpreter::interpreter(interpreter&& interpreter) noexcept
		: talkheui::interpreter(std::move(interpreter)), script_(std::move(interpreter.script_)), result_(std::move(interpreter.result_))
	{}

	interpreter& interpreter::operator=(interpreter&& interpreter) noexcept
	{
		talkheui::interpreter::operator=(std::move(interpreter));
		script_ = std::move(interpreter.script_);
		result_ = std::move(interpreter.result_);
		return *this;
	}

	void interpreter::unload_script()
	{
		script_.clear();
	}

	bool interpreter::is_loaded_script() const
	{
		return !script_.empty();
	}
	void interpreter::run_script()
	{
		while (!result_.has_value())
		{
			run_script_step();
		}
	}
	void interpreter::run_script_step()
	{
		move_cursor();

		runtime_state* const s = static_cast<runtime_state*>(state());
		u5e::basic_grapheme<std::u32string> command_grp = script_.at(s->x_, s->y_);

		if (command_grp.codepoint_end() - command_grp.codepoint_begin() > 1) return;
		const char32_t command = *command_grp.codepoint_begin();
		if (!ishangul(command)) return;

		storage* const storage = static_cast<aheui::storage*>(s->memories()[s->selected_memory()]);
		const jaso command_jaso = get_jaso(command);

		update_cursor(s, command_jaso.jungsung);

		static std::map<char32_t, long long> constants = {
			{ 0, 0 },
			{ U'ㄱ', 2 },
			{ U'ㄴ', 2 },
			{ U'ㄷ', 3 },
			{ U'ㄹ', 5 },
			{ U'ㅁ', 4 },
			{ U'ㅂ', 4 },
			{ U'ㅅ', 2 },
			{ U'ㅈ', 3 },
			{ U'ㅊ', 4 },
			{ U'ㅋ', 3 },
			{ U'ㅌ', 4 },
			{ U'ㅍ', 4 },
			{ U'ㄲ', 4 },
			{ U'ㄳ', 4 },
			{ U'ㄵ', 5 },
			{ U'ㄶ', 5 },
			{ U'ㄺ', 7 },
			{ U'ㄻ', 9 },
			{ U'ㄼ', 9 },
			{ U'ㄽ', 7 },
			{ U'ㄾ', 9 },
			{ U'ㄿ', 9 },
			{ U'ㅀ', 8 },
			{ U'ㅄ', 6 },
			{ U'ㅆ', 4 },
		};
		static std::map<char32_t, int> storages = {
			{ 0, 0 },
			{ U'ㄱ', 1 },
			{ U'ㄲ', 2 },
			{ U'ㄳ', 3 },
			{ U'ㄴ', 4 },
			{ U'ㄵ', 5 },
			{ U'ㄶ', 6 },
			{ U'ㄷ', 7 },
			{ U'ㄹ', 8 },
			{ U'ㄺ', 9 },
			{ U'ㄻ', 10 },
			{ U'ㄼ', 11 },
			{ U'ㄽ', 12 },
			{ U'ㄾ', 13 },
			{ U'ㄿ', 14 },
			{ U'ㅀ', 15 },
			{ U'ㅁ', 16 },
			{ U'ㅂ', 17 },
			{ U'ㅄ', 18 },
			{ U'ㅅ', 19 },
			{ U'ㅆ', 20 },
			{ U'ㅇ', 21 },
			{ U'ㅈ', 22 },
			{ U'ㅊ', 23 },
			{ U'ㅋ', 24 },
			{ U'ㅌ', 25 },
			{ U'ㅍ', 26 },
			{ U'ㅎ', 27 },
		};

		switch (command_jaso.chosung)
		{
		case U'ㅎ':
			if (storage->size() == 0) result_ = 0;
			else result_ = storage->pop();
			break;

		case U'ㄷ':
		case U'ㄸ':
		case U'ㅌ':
		case U'ㄴ':
		case U'ㄹ':
		{
			static std::map<char32_t, long long(*)(long long, long long)> operators = {
				{ U'ㄷ', [](long long a, long long b) { return a + b; } },
				{ U'ㄸ', [](long long a, long long b) { return a * b; } },
				{ U'ㅌ', [](long long a, long long b) { return a - b; } },
				{ U'ㄴ', [](long long a, long long b) { return a / b; } },
				{ U'ㄹ', [](long long a, long long b) { return a % b; } },
			};

			if (storage->size() < 2)
			{
				reverse_cursor();
				break;
			}
			const long long b = storage->pop();
			const long long a = storage->pop();
			storage->push(operators[command_jaso.chosung](a, b));

			break;
		}

		case U'ㅁ':
		{
			if (storage->size() < 1)
			{
				reverse_cursor();
				break;
			}
			
			const long long v = storage->pop();
			if (command_jaso.jongsung == U'ㅇ')
			{
				std::printf("%lld", v);
			}
			else if (command_jaso.jongsung == U'ㅎ')
			{
				if constexpr (sizeof(wchar_t) == 2)
				{
					const std::u16string v_str = utf32to16(std::u32string_view(reinterpret_cast<const char32_t*>(&v), 1));
					std::printf("%ws", reinterpret_cast<const wchar_t*>(v_str.c_str()));
				}
				else
				{
					const std::string v_str = utf32to8(std::u32string_view(reinterpret_cast<const char32_t*>(&v), 1));
					std::printf("%s", reinterpret_cast<const char*>(v_str.c_str()));
				}
			}
			break;
		}
		case U'ㅂ':
		{
			long long v;
			if (command_jaso.jongsung == U'ㅇ')
			{
				std::scanf("%lld", &v);
			}
			else if (command_jaso.jongsung == U'ㅎ')
			{
				if constexpr (sizeof(wchar_t) == 2)
				{
					wchar_t fc;
					std::scanf("%wc", &fc);
					if ((fc & 0xD800) == 0)
					{
						v = fc;
					}
					else
					{
						wchar_t sc;
						std::scanf("%wc", &sc);
						v = (fc - 0xD800) * 0x400 + (sc - 0xDC00);
					}
				}
				else
				{
					unsigned char fb;
					std::scanf("%c", reinterpret_cast<char*>(&fb));
					if (fb < 0x80)
					{
						v = fb;
					}
					else if ((fb & 0xF0) == 0xF0)
					{
						unsigned char sb, tb, frb;
						std::scanf("%c%c%c", reinterpret_cast<char*>(&sb), reinterpret_cast<char*>(&tb), reinterpret_cast<char*>(&frb));
						v = ((fb & 0x07) << 18) + ((sb & 0x3F) << 12) + ((tb & 0x3F) << 6) + (frb & 0x3F);
					}
					else if ((fb & 0xE0) == 0xE0)
					{
						unsigned char sb, tb;
						std::scanf("%c%c", reinterpret_cast<char*>(&sb), reinterpret_cast<char*>(&tb));
						v = ((fb & 0x0F) << 12) + ((sb & 0x3F) << 6) + (tb & 0x3F);
					}
					else
					{
						unsigned char sb;
						std::scanf("%c", reinterpret_cast<char*>(&sb));
						v = ((fb & 0x1F) << 6) + (sb & 0x3F);
					}
				}
			}
			else
			{
				v = constants[command_jaso.jongsung];
			}
			storage->push(v);
			break;
		}
		case U'ㅃ':
		{
			if (storage->size() < 1)
			{
				reverse_cursor();
				break;
			}
			storage->copy();
			break;
		}
		case U'ㅍ':
		{
			if (storage->size() < 2)
			{
				reverse_cursor();
				break;
			}
			storage->swap();
			break;
		}

		case U'ㅅ':
		{
			s->selected_memory_ = static_cast<std::size_t>(storages[command_jaso.jongsung]);
			break;
		}
		case U'ㅆ':
		{
			if (storage->size() < 1)
			{
				reverse_cursor();
				break;
			}
			const long long j = storages[command_jaso.jongsung];
			static_cast<aheui::storage*>(s->memories()[static_cast<std::size_t>(j)])->push(storage->pop());
			break;
		}
		case U'ㅈ':
		{
			if (storage->size() < 2)
			{
				reverse_cursor();
				break;
			}
			const long long b = storage->pop();
			const long long a = storage->pop();
			storage->push(a >= b);
			break;
		}
		case U'ㅊ':
		{
			if (storage->size() < 1)
			{
				reverse_cursor();
				break;
			}
			const long long v = storage->pop();
			if (!v)
			{
				reverse_cursor();
			}
			break;
		}
		}
	}

	void interpreter::construct_pipe(const std::string& path)
	{
		runtime_state* const s = static_cast<runtime_state*>(state());
		s->extension(static_cast<aheui::extension*>(const_cast<talkheui::extension*>(extensions().at(path))));
	}
	void interpreter::deconstruct_pipe()
	{
		runtime_state* const s = static_cast<runtime_state*>(state());
		s->extension(nullptr);
	}

	void interpreter::reset_priv()
	{
		result_.reset();
	}

	void interpreter::load_script_priv(const std::string_view& script)
	{
		const std::u32string script_utf32 = utf8to32(script);
		script_.parse(script_utf32);
	}

	void interpreter::move_cursor()
	{
		runtime_state* const s = static_cast<runtime_state*>(state());

		s->x_ += s->dx_;
		s->y_ += s->dy_;
		
		if (s->dy_ != 0)
		{
			const int ml = static_cast<int>(script_.max_lines());

			if (s->y_ < 0) s->y_ = ml - 1;
			else if (s->y_ >= ml) s->y_ = 0;
		}
		else
		{
			const int w = static_cast<int>(script_.at(s->y_).size());

			if (s->x_ < 0) s->x_ = w - 1;
			else if (s->x_ >= w) s->x_ = 0;
		}
	}
	void interpreter::reverse_cursor()
	{
		runtime_state* const s = static_cast<runtime_state*>(state());

		s->dx_ = -s->dx_;
		s->dy_ = -s->dy_;
	}
	void interpreter::update_cursor(runtime_state* state, char32_t jungsung)
	{
		switch (jungsung)
		{
		case U'ㅏ':
			state->dx_ = 1;
			state->dy_ = 0;
			break;
		case U'ㅑ':
			state->dx_ = 2;
			state->dy_ = 0;
			break;
		case U'ㅓ':
			state->dx_ = -1;
			state->dy_ = 0;
			break;
		case U'ㅕ':
			state->dx_ = -2;
			state->dy_ = 0;
			break;
		case U'ㅗ':
			state->dx_ = 0;
			state->dy_ = -1;
			break;
		case U'ㅛ':
			state->dx_ = 0;
			state->dy_ = -2;
			break;
		case U'ㅜ':
			state->dx_ = 0;
			state->dy_ = 1;
			break;
		case U'ㅠ':
			state->dx_ = 0;
			state->dy_ = 2;
			break;
		case U'ㅡ':
			state->dy_ = -state->dy_;
			break;
		case U'ㅣ':
			state->dx_ = -state->dx_;
			break;
		case U'ㅢ':
			reverse_cursor();
			break;
		}
	}

	const codeplane& interpreter::script() const noexcept
	{
		return script_;
	}
	long long interpreter::result() const noexcept
	{
		return result_.value();
	}
	bool interpreter::has_result() const noexcept
	{
		return result_.has_value();
	}
}