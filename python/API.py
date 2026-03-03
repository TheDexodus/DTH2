from typing import Any, Tuple, Callable, List, Optional


class ClanMemberSkin:
    name: str
    skin_color: int
    foots_color: int
    custom_colors: bool


class ClanMember:
    name: str
    role: str
    skin: ClanMemberSkin

class ClanPlayer:
    name: str
    status: str


class Vector2:
    x: float
    y: float

    def __init__(self, x: float, y: float):
        pass

    def distance(self, vector: 'Vector2') -> float:
        pass

    def normalize(self) -> 'Vector2':
        pass

    def __add__(self, other: 'Vector2') -> 'Vector2':
        pass

    def __sub__(self, other: 'Vector2') -> 'Vector2':
        pass

    def __mul__(self, other: float) -> 'Vector2':
        pass

    def __div__(self, other: float) -> 'Vector2':
        pass

class World:
    def get_game_tick(self) -> int:
        pass

    def predict(self, ticks: int) -> 'World':
        pass

    def get_player(self, player_id: int) -> 'Player':
        pass

    def input_move(self, player_id: int, direction: int) -> None:
        pass

    def input_set_target(self, player_id: int, target_pos: 'Vector2') -> None:
        pass

    def input_jump(self, player_id: int) -> None:
        pass

    def input_fire(self, player_id: int) -> None:
        pass

    def input_hook(self, player_id: int, hook: bool) -> None:
        pass


class Tee:
    pos: Vector2
    vel: Vector2
    hook_pos: Vector2
    hook_dir: Vector2
    hook_tele_base: Vector2
    hook_tick: int
    hook_state: int
    hooked_player: int
    active_weapon: int
    is_new_hook: bool
    jumped: int
    jumped_total: int
    jumps: int
    direction: int
    angle: int
    triggered_events: int
    id: int
    is_reset: bool
    last_vel: Vector2
    colliding: int
    is_left_wall: bool
    is_solo: bool
    is_jetpack: bool
    is_collision_disabled: bool
    is_endless_hook: bool
    is_endless_jump: bool
    is_hammer_hit_disabled: bool
    is_grenade_hit_disabled: bool
    is_laser_hit_disabled: bool
    is_shotgun_hit_disabled: bool
    is_hook_hit_disabled: bool
    is_super: bool
    has_telegun_gun: bool
    has_telegun_grenade: bool
    has_telegun_laser: bool
    freeze_start: int
    freeze_end: int
    is_in_freeze: bool
    is_deep_frozen: bool
    is_live_frozen: bool
    input_direction: int
    input_target: Vector2
    input_jump: int
    input_fire: int
    input_hook: int
    input_player_flags: int
    input_wanted_weapon: int
    input_next_weapon: int
    input_prev_weapon: int

    def get_size(self) -> float:
        pass

    def predict(self, ticksOffset: int) -> 'Tee':
        pass


class Player:
    use_custom_color: int
    color_body: int
    color_feet: int
    name: str
    clan: str
    country: int
    skin_name: str
    skin_color: int
    team: int
    emoticon: int
    emoticon_start_fraction: float
    emoticon_start_tick: int
    is_solo: bool
    is_jetpack: bool
    is_collision_disabled: bool
    is_endless_hook: bool
    is_endless_jump: bool
    is_hammer_hit_disabled: bool
    is_grenade_hit_disabled: bool
    is_laser_hit_disabled: bool
    is_shotgun_hit_disabled: bool
    is_hook_hit_disabled: bool
    is_super: bool
    has_telegun_gun: bool
    has_telegun_grenade: bool
    has_telegun_laser: bool
    freeze_end: int
    is_deep_frozen: bool
    is_live_frozen: bool
    angle: float
    is_active: bool
    is_chat_ignore: bool
    is_emoticon_ignore: bool
    is_friend: bool
    is_foe: bool
    auth_level: int
    is_afk: bool
    is_paused: bool
    is_spec: bool
    render_pos: Vector2
    is_predicted: bool
    is_predicted_local: bool
    smooth_start: Tuple[int, int]
    smooth_len: Tuple[int, int]
    is_spec_char_present: bool
    spec_char: Vector2
    tee: Tee

    def __init__(self, playerId: int):
        pass


def LocalID(clientId: int) -> int:
    pass

def Timeout(timeoutSeconds: float, callback: Callable[[], None]) -> None:
    pass

class Collision:
    @staticmethod
    def intersect_line(position0: Vector2, position1: Vector2) -> Tuple[int, Vector2, Vector2]:
        pass

    @staticmethod
    def intersect_line_tele_hook(position0: Vector2, position1: Vector2) -> Tuple[int, Vector2, Vector2]:
        pass

    @staticmethod
    def get_map_size() -> Vector2:
        pass

    @staticmethod
    def get_tile(position: Vector2) -> int:
        pass

class Chat:
    @staticmethod
    def send_local_message(message: Any):
        pass;

    @staticmethod
    def send_message(message: Any):
        pass;

class Clan:
    @staticmethod
    def get_members() -> List[ClanMember]:
        pass

    @staticmethod
    def get_players() -> List[ClanPlayer]:
        pass

class Console:
    @staticmethod
    def debug(message: str) -> None:
        pass


Direction = {"Left": -1, "None": 0, "Right": 1}


class Input:

    @staticmethod
    def move(direction: Direction) -> None:
        pass

    @staticmethod
    def jump() -> None:
        pass

    @staticmethod
    def hook(hook: bool) -> None:
        pass

    @staticmethod
    def fire() -> None:
        pass

    @staticmethod
    def set_block_user_input(block: bool) -> None:
        pass

    @staticmethod
    def show_cursor(is_visible: bool) -> None:
        pass

    @staticmethod
    def set_target(position: Vector2) -> None:
        pass

    @staticmethod
    def set_target_human_like(position: Vector2, move_time: float, on_arrived: Callable[[], None] = None) -> None:
        pass

    @staticmethod
    def move_mouse_to_player(player_id: int, move_time: float, on_arrived: Callable[[], None] = None) -> None:
        pass

    @staticmethod
    def is_human_like_move_ended() -> bool:
        pass

    @staticmethod
    def get_mouse_position() -> Vector2:
        pass

    @staticmethod
    def get_cursor_position() -> Vector2:
        pass

    @staticmethod
    def get_target_position() -> Vector2:
        pass

    @staticmethod
    def remove_moving() -> None:
        pass

    @staticmethod
    def set_wanted_weapon(weapon: int):
        pass

    @staticmethod
    def reset():
        pass
        pass

    @staticmethod
    def cancel_human_like():
        pass

    @staticmethod
    def kill() -> None:
        pass

    @staticmethod
    def swap() -> None:
        pass


class DummyInput:
    @staticmethod
    def move(direction: Direction) -> None:
        pass

    @staticmethod
    def jump() -> None:
        pass

    @staticmethod
    def hook(hook: bool) -> None:
        pass

    @staticmethod
    def fire() -> None:
        pass

    @staticmethod
    def set_target(position: Vector2) -> None:
        pass

    @staticmethod
    def get_mouse_position() -> Vector2:
        pass

    @staticmethod
    def get_target_position() -> Vector2:
        pass

    @staticmethod
    def set_wanted_weapon(weapon: int):
        pass

    @staticmethod
    def enable_control(enable: bool) -> None:
        pass

    @staticmethod
    def kill() -> None:
        pass


class Time:
    @staticmethod
    def get_local_time() -> float:
        pass

    @staticmethod
    def get_game_tick(local_player_id: int = None) -> int:
        pass


class Predict:
    @staticmethod
    def predict_laser(shooter: Player, shoot_position: Vector2) -> Tuple[List[Vector2], Optional[Player]]:
        pass

class Character:
    player_flags: int
    health: int
    armor: int
    ammo_count: int
    weapon: int
    emote: int
    attack_tick: int

    is_active: bool
    position: Vector2

    def __init__(self, playerId: int):
        pass

class Graphics:
    @staticmethod
    def draw_circle(position: Vector2, radius: float, color: int = 0xffffffff) -> int:
        pass

    @staticmethod
    def draw_line(from_position: Vector2, to_position: Vector2, color: int = 0xffffffff) -> int:
        pass

    @staticmethod
    def draw_text(pos: Vector2, font_weight: float, text: str, color: int = 0xffffffff) -> int:
        pass

    @staticmethod
    def get_rgba(red: int, green: int, blue: int, alpha: int = 255) -> int:
        pass

    @staticmethod
    def draw_rect(position_start: Vector2, position_end: Vector2, filled: bool, color: int = 0xffffffff) -> int:
        pass

    @staticmethod
    def draw_rounded_rect(position_start: Vector2, position_end: Vector2, filled: bool, border_radius: float, color: int = 0xffffffff) -> int:
        pass

    @staticmethod
    def draw_sprite(position: Vector2, sprite_path: str, size: float) -> int:
        pass

    @staticmethod
    def get_screen_size() -> Vector2:
        pass

    @staticmethod
    def remove_draw_object(object_id: int) -> None:
        pass

    @staticmethod
    def move_draw_object(object_id: int, new_coordinate: Vector2) -> None:
        pass

    @staticmethod
    def get_draw_object_position(object_id: int) -> Optional[Vector2]:
        pass

    @staticmethod
    def set_draw_object_rotation(object_id: int, degree: float) -> None:
        pass

    @staticmethod
    def get_draw_object_rotation(object_id: int) -> float:
        pass

    @staticmethod
    def map_world_position_to_ui(world_position: Vector2) -> Vector2:
        pass

    @staticmethod
    def map_ui_position_to_world(ui_position: Vector2) -> Vector2:
        pass

    @staticmethod
    def get_text_width(font_weight: float, text: str) -> float:
        pass

    @staticmethod
    def is_menu_displayed() -> bool:
        pass

class Tuning:
    @staticmethod
    def get_ground_control_speed() -> float:
        pass

    @staticmethod
    def get_ground_control_accel() -> float:
        pass

    @staticmethod
    def get_ground_friction() -> float:
        pass

    @staticmethod
    def get_ground_jump_impulse() -> float:
        pass

    @staticmethod
    def get_air_jump_impulse() -> float:
        pass

    @staticmethod
    def get_air_control_speed() -> float:
        pass

    @staticmethod
    def get_air_control_accel() -> float:
        pass

    @staticmethod
    def get_air_friction() -> float:
        pass

    @staticmethod
    def get_hook_length() -> float:
        pass

    @staticmethod
    def get_hook_fire_speed() -> float:
        pass

    @staticmethod
    def get_hook_drag_accel() -> float:
        pass

    @staticmethod
    def get_hook_drag_speed() -> float:
        pass

    @staticmethod
    def get_gravity() -> float:
        pass

    @staticmethod
    def get_velramp_start() -> float:
        pass

    @staticmethod
    def get_velramp_range() -> float:
        pass

    @staticmethod
    def get_velramp_curvature() -> float:
        pass

    @staticmethod
    def get_gun_curvature() -> float:
        pass

    @staticmethod
    def get_gun_speed() -> float:
        pass

    @staticmethod
    def get_gun_lifetime() -> float:
        pass

    @staticmethod
    def get_shotgun_curvature() -> float:
        pass

    @staticmethod
    def get_shotgun_speed() -> float:
        pass

    @staticmethod
    def get_shotgun_speeddiff() -> float:
        pass

    @staticmethod
    def get_shotgun_lifetime() -> float:
        pass

    @staticmethod
    def get_grenade_curvature() -> float:
        pass

    @staticmethod
    def get_grenade_speed() -> float:
        pass

    @staticmethod
    def get_grenade_lifetime() -> float:
        pass

    @staticmethod
    def get_laser_reach() -> float:
        pass

    @staticmethod
    def get_laser_bounce_delay() -> float:
        pass

    @staticmethod
    def get_laser_bounce_num() -> float:
        pass

    @staticmethod
    def get_laser_bounce_cost() -> float:
        pass

    @staticmethod
    def get_laser_damage() -> float:
        pass

    @staticmethod
    def get_player_collision() -> float:
        pass

    @staticmethod
    def get_player_hooking() -> float:
        pass

    @staticmethod
    def get_jetpack_strength() -> float:
        pass

    @staticmethod
    def get_shotgun_strength() -> float:
        pass

    @staticmethod
    def get_explosion_strength() -> float:
        pass

    @staticmethod
    def get_hammer_strength() -> float:
        pass

    @staticmethod
    def get_hook_duration() -> float:
        pass

    @staticmethod
    def get_hammer_fire_delay() -> float:
        pass

    @staticmethod
    def get_gun_fire_delay() -> float:
        pass

    @staticmethod
    def get_shotgun_fire_delay() -> float:
        pass

    @staticmethod
    def get_grenade_fire_delay() -> float:
        pass

    @staticmethod
    def get_laser_fire_delay() -> float:
        pass

    @staticmethod
    def get_ninja_fire_delay() -> float:
        pass

    @staticmethod
    def get_hammer_hit_fire_delay() -> float:
        pass

    @staticmethod
    def get_ground_elasticity_x() -> float:
        pass

    @staticmethod
    def get_ground_elasticity_y() -> float:
        pass

#######################################################
# For using events, just write in your code function. #
# for example:                                        #
#   def on_update():                                  #
#       ...your_code                                  #
#######################################################
class Events:
    @staticmethod
    def on_update() -> None:
        pass

    @staticmethod
    def on_input(key_code: int, flags: int, key_name: str) -> Optional[bool]:
        pass

    @staticmethod
    def on_script_started() -> None:
        pass

    @staticmethod
    def on_script_stopped() -> None:
        pass

    @staticmethod
    def get_script_name() -> str:
        pass

    @staticmethod
    def on_message(client_id: int, team: int, message: str) -> None:
        pass


