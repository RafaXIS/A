#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

LINK_ENTITY_TO_CLASS(weapon_hflashlight, CHFlashlight);

enum gauss_e {
	HFLASHLIGHT_IDLE = 0,
	HFLASHLIGHT_DRAW,
	HFLASHLIGHT_HOLSTER,
	HFLASHLIGHT_FlashlightFire1,
};

void CHFlashlight::Spawn()
{
	pev->classname = MAKE_STRING("weapon_hflashlight"); // hack to allow for old names
	Precache();
	SET_MODEL(ENT(pev), "models/w_flashlight.mdl"); // This should be your own weapon model
	m_iId = WEAPON_HFLASHLIGHT;

	m_iDefaultAmmo = 50; // this could be replaced with EXAMPLE_DEFAULT_GIVE if you had defined it

	FallInit();// get ready to fall down.

}


void CHFlashlight::Precache(void)
{

	// Again, you must replace these precache models and sounds with your own models and sounds
	PRECACHE_MODEL("models/v_flashlight.mdl");
	PRECACHE_MODEL("models/w_flashlight.mdl");
	PRECACHE_MODEL("models/p_flashlight.mdl");

	PRECACHE_SOUND("items/flash_battery1.wav");

	PRECACHE_SOUND("items/flash_ins1.wav");
	PRECACHE_SOUND("items/flash_release1.wav");

	PRECACHE_SOUND("weapons/flash_click1.wav");
	PRECACHE_SOUND("weapons/flash_click2.wav");

	// this is to hook your client-side event
	m_usFlashlightFire1 = PRECACHE_EVENT(1, "events/hflashlight.sc");
}

int CHFlashlight::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "battery";
	p->iMaxAmmo1 = 200;
	p->iMaxClip = -1; // means that there is no clip
	p->iSlot = 0;
	p->iPosition = 1;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_HFLASHLIGHT;
	p->iWeight = MP5_WEIGHT; // weight is for autoswitching, so it doesn't really matter much
	p->pszAmmo2 = NULL; // no secondary ammo
	p->iMaxAmmo2 = -1;

	return 1;
}

int CHFlashlight::AddToPlayer(CBasePlayer* pPlayer)
{
	if (CBasePlayerWeapon::AddToPlayer(pPlayer))
	{
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

void CHFlashlight::PrimaryAttack()
{
	if (m_pPlayer->FlashlightIsOn())
	{
		m_pPlayer->FlashlightTurnOff();//OFF
	}
	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = GetNextAttackDelay(0.25);

	if (m_flNextPrimaryAttack < UTIL_WeaponTimeBase())
	{
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25;
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

void CHFlashlight::SecondaryAttack()
{
}

BOOL CHFlashlight::Deploy()
{
	return DefaultDeploy("models/v_flashlight.mdl", "models/p_flashlight.mdl", HFLASHLIGHT_DRAW, "hflashlight");
}

void CHFlashlight::WeaponIdle()
{
	Reload();

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	int iAnim;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if (flRand <= 0.75)
	{
		iAnim = HFLASHLIGHT_IDLE;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 30.0 / 16 * (2);
	}
	SendWeaponAnim(iAnim);
}


	