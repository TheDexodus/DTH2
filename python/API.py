from typing import Any, Tuple, Callable, List, Optional


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


class Tee:
    pos: Vector2
    vel: Vector2
    hookPos: Vector2
    hookDir: Vector2
    hookTeleBase: Vector2
    hookTick: int
    hookState: int
    hookedPlayer: int
    activeWeapon: int
    isNewHook: bool
    jumped: int
    jumpedTotal: int
    jumps: int
    direction: int
    angle: int
    triggeredEvents: int
    id: int
    isReset: bool
    lastVel: Vector2
    colliding: int
    isLeftWall: bool
    isSolo: bool
    isJetpack: bool
    isCollisionDisabled: bool
    isEndlessHook: bool
    isEndlessJump: bool
    isHammerHitDisabled: bool
    isGrenadeHitDisabled: bool
    isLaserHitDisabled: bool
    isShotgunHitDisabled: bool
    isHookHitDisabled: bool
    isSuper: bool
    hasTelegunGun: bool
    hasTelegunGrenade: bool
    hasTelegunLaser: bool
    freezeStart: int
    freezeEnd: int
    isInFreeze: bool
    isDeepFrozen: bool
    isLiveFrozen: bool

    def getSize(self) -> float:
        pass

    def predict(self, ticksOffset: int) -> 'Tee':
        pass


class Player:
    useCustomColor: int
    colorBody: int
    colorFeet: int
    name: str
    clan: str
    country: int
    skinName: str
    skinColor: int
    team: int
    emoticon: int
    emoticonStartFraction: float
    emoticonStartTick: int
    isSolo: bool
    isJetpack: bool
    isCollisionDisabled: bool
    isEndlessHook: bool
    isEndlessJump: bool
    isHammerHitDisabled: bool
    isGrenadeHitDisabled: bool
    isLaserHitDisabled: bool
    isShotgunHitDisabled: bool
    isHookHitDisabled: bool
    isSuper: bool
    isHasTelegunGun: bool
    isHasTelegunGrenade: bool
    isHasTelegunLaser: bool
    freezeEnd: int
    isDeepFrozen: bool
    isLiveFrozen: bool
    angle: float
    isActive: bool
    isChatIgnore: bool
    isEmoticonIgnore: bool
    isFriend: bool
    isFoe: bool
    authLevel: int
    isAfk: bool
    isPaused: bool
    isSpec: bool
    renderPos: Vector2
    isPredicted: bool
    isPredictedLocal: bool
    smoothStart: Tuple[int, int]
    smoothLen: Tuple[int, int]
    isSpecCharPresent: bool
    specChar: Vector2
    tee: Tee

    def __init__(self, playerId: int):
        pass


def LocalID(clientId: int) -> int:
    pass

def Timeout(timeoutSeconds: float, callback: Callable[[], None]) -> None:
    pass

class Collision:
    @staticmethod
    def intersectLine(position0: Vector2, position1: Vector2) -> Tuple[int, Vector2, Vector2]:
        pass

    @staticmethod
    def getMapSize() -> Vector2:
        pass

    @staticmethod
    def getTile(position: Vector2) -> int:
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
    def setBlockUserInput(block: bool) -> None:
        pass

    @staticmethod
    def setTarget(position: Vector2) -> None:
        pass

    @staticmethod
    def setTargetHumanLike(position: Vector2, moveTime: float, onArrived: Callable[[], None] = None) -> None:
        pass

    @staticmethod
    def moveMouseToPlayer(playerId: int, moveTime: float, onArrived: Callable[[], None] = None) -> None:
        pass

    @staticmethod
    def isHumanLikeMoveEnded() -> bool:
        pass

    @staticmethod
    def getMousePosition() -> Vector2:
        pass

    @staticmethod
    def getTargetPosition() -> Vector2:
        pass

    @staticmethod
    def removeMoving() -> None:
        pass

    @staticmethod
    def setWantedWeapon(weapon: int):
        pass

    @staticmethod
    def reset():
        pass
        pass

    @staticmethod
    def cancelHumanLike():
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
    def setTarget(position: Vector2) -> None:
        pass

    @staticmethod
    def getMousePosition() -> Vector2:
        pass

    @staticmethod
    def getTargetPosition() -> Vector2:
        pass

    @staticmethod
    def setWantedWeapon(weapon: int):
        pass

    @staticmethod
    def enableControl(enable: bool) -> None:
        pass


class Time:
    @staticmethod
    def getLocalTime() -> float:
        pass

    @staticmethod
    def getGameTick(localPlayerId: int = None) -> int:
        pass


class Predict:
    @staticmethod
    def predictLaser(shooter: Player, shootPosition: Vector2) -> Tuple[List[Vector2], Optional[Player]]:
        pass


class Character:
    playerFlags: int
    health: int
    armor: int
    ammoCount: int
    weapon: int
    emote: int
    attackTick: int

    isActive: bool
    position: Vector2

    def __init__(self, playerId: int):
        pass

class Graphics:
    @staticmethod
    def drawCircle(position: Vector2, radius: float, color: int = 0xffffffff) -> int:
        pass

    @staticmethod
    def drawLine(fromPosition: Vector2, toPosition: Vector2, color: int = 0xffffffff) -> int:
        pass

    @staticmethod
    def rgba(red: int, green: int, blue: int, alpha: int = 255) -> int:
        pass

    @staticmethod
    def getScreenSize() -> Vector2:
        pass

    @staticmethod
    def removeDrawObject(objectId: int) -> None:
        pass

    @staticmethod
    def mapWorldPositionToUI(worldPosition: Vector2) -> Vector2:
        pass

class Tuning:
    @staticmethod
    def getGroundControlSpeed() -> float:
        pass

    @staticmethod
    def getGroundControlAccel() -> float:
        pass

    @staticmethod
    def getGroundFriction() -> float:
        pass

    @staticmethod
    def getGroundJumpImpulse() -> float:
        pass

    @staticmethod
    def getAirJumpImpulse() -> float:
        pass

    @staticmethod
    def getAirControlSpeed() -> float:
        pass

    @staticmethod
    def getAirControlAccel() -> float:
        pass

    @staticmethod
    def getAirFriction() -> float:
        pass

    @staticmethod
    def getHookLength() -> float:
        pass

    @staticmethod
    def getHookFireSpeed() -> float:
        pass

    @staticmethod
    def getHookDragAccel() -> float:
        pass

    @staticmethod
    def getHookDragSpeed() -> float:
        pass

    @staticmethod
    def getGravity() -> float:
        pass

    @staticmethod
    def getVelrampStart() -> float:
        pass

    @staticmethod
    def getVelrampRange() -> float:
        pass

    @staticmethod
    def getVelrampCurvature() -> float:
        pass

    @staticmethod
    def getGunCurvature() -> float:
        pass

    @staticmethod
    def getGunSpeed() -> float:
        pass

    @staticmethod
    def getGunLifetime() -> float:
        pass

    @staticmethod
    def getShotgunCurvature() -> float:
        pass

    @staticmethod
    def getShotgunSpeed() -> float:
        pass

    @staticmethod
    def getShotgunSpeeddiff() -> float:
        pass

    @staticmethod
    def getShotgunLifetime() -> float:
        pass

    @staticmethod
    def getGrenadeCurvature() -> float:
        pass

    @staticmethod
    def getGrenadeSpeed() -> float:
        pass

    @staticmethod
    def getGrenadeLifetime() -> float:
        pass

    @staticmethod
    def getLaserReach() -> float:
        pass

    @staticmethod
    def getLaserBounceDelay() -> float:
        pass

    @staticmethod
    def getLaserBounceNum() -> float:
        pass

    @staticmethod
    def getLaserBounceCost() -> float:
        pass

    @staticmethod
    def getLaserDamage() -> float:
        pass

    @staticmethod
    def getPlayerCollision() -> float:
        pass

    @staticmethod
    def getPlayerHooking() -> float:
        pass

    @staticmethod
    def getJetpackStrength() -> float:
        pass

    @staticmethod
    def getShotgunStrength() -> float:
        pass

    @staticmethod
    def getExplosionStrength() -> float:
        pass

    @staticmethod
    def getHammerStrength() -> float:
        pass

    @staticmethod
    def getHookDuration() -> float:
        pass

    @staticmethod
    def getHammerFireDelay() -> float:
        pass

    @staticmethod
    def getGunFireDelay() -> float:
        pass

    @staticmethod
    def getShotgunFireDelay() -> float:
        pass

    @staticmethod
    def getGrenadeFireDelay() -> float:
        pass

    @staticmethod
    def getLaserFireDelay() -> float:
        pass

    @staticmethod
    def getNinjaFireDelay() -> float:
        pass

    @staticmethod
    def getHammerHitFireDelay() -> float:
        pass

    @staticmethod
    def getGroundElasticityX() -> float:
        pass

    @staticmethod
    def getGroundElasticityY() -> float:
        pass

#######################################################
# For using events, just write in your code function. #
# for example:                                        #
#   def onUpdate():                                   #
#       ...your_code                                  #
#######################################################
class Events:
    @staticmethod
    def onUpdate() -> None:
        pass

    # If you want reject user input, return `True` in `onInput`
    @staticmethod
    def onInput(keyCode: int, flags: int, keyName: str) -> Optional[bool]:
        pass

    @staticmethod
    def onScriptStarted() -> None:
        pass

    @staticmethod
    def onScriptStopped() -> None:
        pass

    @staticmethod
    def getScriptName() -> str:
        pass
