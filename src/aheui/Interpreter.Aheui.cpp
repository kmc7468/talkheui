#include <th/aheui/Interpreter.hpp>

#include <th/Encoding.hpp>
#include <th/Hangul.hpp>
#include <th/IOStream.hpp>
#include <th/aheui/Extension.hpp>
#include <th/aheui/Storage.hpp>

#include <cstdio>
#include <unordered_map>

#include <u5e/basic_grapheme.hpp>

namespace th::aheui {
	RuntimeState::RuntimeState() {
		Reset();
	}

	void RuntimeState::Reset() {
		m_Memories.clear();
		ResetStep();
		SelectedStorage = 0;

		for (int i = 0; i < 27; ++i) {
			m_Memories.push_back(new Stack());
		}

		m_Memories.insert(m_Memories.begin() + 21, new Queue());
	}
	void RuntimeState::ResetStep() {
		X = 0;
		Y = -1;
		DX = 0;
		DY = 1;
	}

	aheui::Extension* RuntimeState::GetConstructedExtension() const noexcept {
		return m_Extension;
	}
	void RuntimeState::ConstructPipe(aheui::Extension* extension) {
		delete m_Memories.back();

		if (extension) {
			m_Memories.back() = new Pipe(extension);
		} else {
			m_Memories.back() = new Stack();
		}

		m_Extension = extension;
	}
}

namespace th::aheui {
	Interpreter::Interpreter()
		: th::Interpreter("Aheui") {
		State(new RuntimeState());
	}
	Interpreter::Interpreter(Interpreter&& interpreter) noexcept
		: th::Interpreter(std::move(interpreter)), m_Script(std::move(interpreter.m_Script)), m_Result(std::move(interpreter.m_Result)) {
	}

	Interpreter& Interpreter::operator=(Interpreter&& interpreter) noexcept {
		th::Interpreter::operator=(std::move(interpreter));

		m_Script = std::move(interpreter.m_Script);
		m_Result = std::move(interpreter.m_Result);
		return *this;
	}

	bool Interpreter::IsScriptLoaded() const {
		return !m_Script.IsEmpty();
	}
	void Interpreter::UnloadScript() {
		m_Script.Clear();
		m_Result.reset();
	}
	void Interpreter::RunScript() {
		m_Result.reset();

		while (!HasResult()) {
			RunScriptStep();
		}
	}
	void Interpreter::RunScriptStep() {
		MoveCursor();

		RuntimeState* const s = static_cast<RuntimeState*>(State());
		Storage* const storage = static_cast<aheui::Storage*>(s->m_Memories[s->SelectedStorage]);
		
		u5e::basic_grapheme<std::u32string> commandGrp = m_Script.At(s->X, s->Y);
		if (commandGrp.codepoint_end() - commandGrp.codepoint_begin() >= 2) return;
		const char32_t command = *commandGrp.codepoint_begin();
		if (!IsHangul(command)) return;
		const Jaso commandJaso = GetJaso(command);

		UpdateCursor(commandJaso.Jungsung);

		static std::unordered_map<char32_t, long long> constants = {
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
		static std::unordered_map<char32_t, int> storages = {
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

		switch (commandJaso.Chosung) {
		case U'ㅎ':
			if (storage->Count()) {
				m_Result = storage->Pop();
			} else {
				m_Result = 0;
			}
			break;

		case U'ㄷ':
		case U'ㄸ':
		case U'ㅌ':
		case U'ㄴ':
		case U'ㄹ': {
			static std::unordered_map<char32_t, long long(*)(long long, long long)> operators = {
				{ U'ㄷ', [](long long a, long long b) { return a + b; } },
				{ U'ㄸ', [](long long a, long long b) { return a * b; } },
				{ U'ㅌ', [](long long a, long long b) { return a - b; } },
				{ U'ㄴ', [](long long a, long long b) { return a / b; } },
				{ U'ㄹ', [](long long a, long long b) { return a % b; } },
			};

			if (storage->Count() < 2) {
				ReverseCursor();
			} else {
				const long long b = storage->Pop();
				const long long a = storage->Pop();
				storage->Push(operators[commandJaso.Chosung](a, b));
			}
			break;
		}

		case U'ㅁ': {
			if (storage->Count() < 1) {
				ReverseCursor();
				break;
			}

			long long v = storage->Pop();
			if (commandJaso.Jongsung == U'ㅇ') {
				WriteStdout(v);
			} else if (commandJaso.Jongsung == U'ㅎ') {
				WriteStdout(static_cast<char32_t>(v));
			}
			break;
		}

		case U'ㅂ': {
			long long v;
			if (commandJaso.Jongsung == U'ㅇ') {
				v = ReadIntegerStdin();
			} else if (commandJaso.Jongsung == U'ㅎ') {
				v = ReadCharacterStdin();
			} else {
				v = constants[commandJaso.Jongsung];
			}
			storage->Push(v);
			break;
		}

		case U'ㅃ':
			if (storage->Count() < 1) {
				ReverseCursor();
			} else {
				storage->Copy();
			}
			break;

		case U'ㅍ':
			if (storage->Count() < 2) {
				ReverseCursor();
			} else {
				storage->Swap();
			}
			break;

		case U'ㅅ':
			s->SelectedStorage = static_cast<std::size_t>(storages[commandJaso.Jongsung]);
			break;

		case U'ㅆ': {
			if (storage->Count() < 1) {
				ReverseCursor();
			} else {
				const int to = storages[commandJaso.Jongsung];
				static_cast<aheui::Storage*>(s->m_Memories[static_cast<std::size_t>(to)])->Push(storage->Pop());
			}
			break;
		}

		case U'ㅈ': {
			if (storage->Count() < 2) {
				ReverseCursor();
			} else {
				const long long b = storage->Pop();
				const long long a = storage->Pop();
				storage->Push(a >= b);
			}
			break;
		}

		case U'ㅊ':
			if (storage->Count() < 1 || !storage->Pop()) {
				ReverseCursor();
			}
			break;
		}
	}

	void Interpreter::ConstructPipe(const std::string& path) {
		RuntimeState* const s = static_cast<RuntimeState*>(State());
		s->ConstructPipe(static_cast<aheui::Extension*>(const_cast<th::Extension*>(Extensions().at(path))));
	}
	void Interpreter::DeconstructPipe() {
		RuntimeState* const s = static_cast<RuntimeState*>(State());
		s->ConstructPipe(nullptr);
	}

	void Interpreter::vReset() {
		m_Result.reset();
	}

	void Interpreter::vLoadScript(const std::string_view& script) {
		const std::u32string scriptUTF32 = UTF8To32(script);
		m_Script.Parse(scriptUTF32);
	}

	void Interpreter::MoveCursor() {
		RuntimeState* const s = static_cast<RuntimeState*>(State());
		s->X += s->DX;
		s->Y += s->DY;

		if (s->DY) {
			const int ml = static_cast<int>(m_Script.MaxLines());
			if (s->Y < 0) s->Y = ml - 1;
			else if (s->Y >= ml) s->Y = 0;
		} else {
			const int w = static_cast<int>(m_Script.At(s->Y).size());
			if (s->X < 0) s->X = w - 1;
			else if (s->X >= w) s->X = 0;
		}
	}
	void Interpreter::ReverseCursor() {
		RuntimeState* const s = static_cast<RuntimeState*>(State());
		s->DX = -s->DX;
		s->DY = -s->DY;
	}
	void Interpreter::UpdateCursor(char32_t jungsung) {
		RuntimeState* const s = static_cast<RuntimeState*>(State());
		switch (jungsung) {
		case U'ㅏ':
			s->DX = 1;
			s->DY = 0;
			break;
		case U'ㅑ':
			s->DX = 2;
			s->DY = 0;
			break;
		case U'ㅓ':
			s->DX = -1;
			s->DY = 0;
			break;
		case U'ㅕ':
			s->DX = -2;
			s->DY = 0;
			break;
		case U'ㅗ':
			s->DX = 0;
			s->DY = -1;
			break;
		case U'ㅛ':
			s->DX = 0;
			s->DY = -2;
			break;
		case U'ㅜ':
			s->DX = 0;
			s->DY = 1;
			break;
		case U'ㅠ':
			s->DX = 0;
			s->DY = 2;
			break;
		case U'ㅡ':
			s->DY = -s->DY;
			break;
		case U'ㅣ':
			s->DX = -s->DX;
			break;
		case U'ㅢ':
			ReverseCursor();
			break;
		}
	}

	const CodePlane& Interpreter::Script() const noexcept {
		return m_Script;
	}
	long long Interpreter::Result() const noexcept {
		return m_Result.value();
	}
	bool Interpreter::HasResult() const noexcept {
		return m_Result.has_value();
	}
}