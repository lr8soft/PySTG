from .OvalTypeBullet import OvalTypeBullet, BulletColor


class OvalBullet(OvalTypeBullet):
    def __init__(self):
        super().__init__()
        super()._setDivideInfo(divideType=[1, 8], divideOffset=[0.0, 0.0])
        super()._setSelectImage(coord=[1, 6])
        super()._setCollideSize([0.055, 0.0275, 0.055])
        super()._setRenderSize([0.055, 0.055])

        super().setBulletImage("assets/Bullet/ovalBullet.png")

        super()._setReleaseParticleDensity(10)
        super()._setReleaseParticleLifeTime(0.5)
        super()._setReleaseParticleSize(15)
        super()._setReleaseParticleVelocity(0.6)

    def setBulletColor(self, color=BulletColor.BLUE):
        super()._setSelectImage(coord=[1, 9 - color.value])
        self._ovalBullet_setReleaseColor(color)
