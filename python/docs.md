### Python API: события скрипта

Все имена функций и методов используются в `snake_case`.

#### Поддерживаемые callbacks (определяются в корне скрипта)

- `def get_script_name() -> str`  
  Вызывается при загрузке скрипта. Возвращаемая строка используется как имя скрипта.

- `def on_script_started() -> None`  
  Вызывается при запуске скрипта.

- `def on_script_stopped() -> None`  
  Вызывается при остановке скрипта.

- `def on_update() -> None`  
  Вызывается каждый тик обновления.

- `def on_input(key_code: int, flags: int, key_name: str) -> bool | None`  
  Вызывается при вводе. Если вернуть `True`, событие считается обработанным.

- `def on_message(client_id: int, team: int, message: str) -> None`  
  Вызывается при получении сообщения.

#### Важно

- Старые camelCase имена (`getScriptName`, `onUpdate`, `onInput`, ...) больше не используются.
- Используйте только `snake_case` методы API, например:
  - `API.Input.show_cursor(...)`
  - `API.Input.get_cursor_position()`
  - `API.Graphics.is_menu_displayed()`
  - `API.Graphics.draw_text(...)`
