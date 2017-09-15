package player;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.HashMap;

import static org.junit.jupiter.api.Assertions.*;

class PlayerTest {
    Player player1;
    Player player2;

    static HashMap<String, Skill> skills;
    static HashMap<String, Weapon> weapons;

    @BeforeAll
    public static void setUpSkillsAndWeapons(){
        skills = new HashMap<>();
        skills.put("FireBall", new AttackedSkill(5, 20));
        skills.put("Healing", new HealingSkill(20, 10));

        weapons = new HashMap<>();
        weapons.put("LongSword", new Weapon(10));
        weapons.put("ThunderSword", new Weapon(12, new AttackedSkill(0, 25)));
        weapons.put("SwordOfHealing", new Weapon(10, new HealingSkill(5, 5)));
    }

    @Test
    public void testWeaponsSetUp(){
        assertEquals(10, weapons.get("LongSword").getAtk());
        assertEquals(12, weapons.get("ThunderSword").getAtk());
        assertEquals(10, weapons.get("SwordOfHealing").getAtk());
        assertEquals("", weapons.get("LongSword").skillString());
        assertEquals("AttackedSkill{damage=25, mpUsing=0}", weapons.get("ThunderSword").skillString());
        assertEquals("HealingSkill{healingPoint=5, mpUsing=5}", weapons.get("SwordOfHealing").skillString());
    }

    @Test
    public void testSkillsSetup(){
        assertEquals("AttackedSkill{damage=20, mpUsing=5}", skills.get("FireBall").toString());
        assertEquals("HealingSkill{healingPoint=10, mpUsing=20}", skills.get("Healing").toString());
    }

    @BeforeEach
    public void setUp(){
        player1 = new Player(new Status(100,20,10),
                weapons.get("LongSword"));
        player2 = new Player(new Status(160, 40, 2),
                weapons.get("ThunderSword"));
    }

    @Test
    public void testSetUp(){
        assertEquals("Status{hp: 100/100, mp: 20/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 160/160, mp: 40/40, atk=2}", player2.statusToString());
        assertEquals("{}", player1.skillsToString());
        assertEquals("{}", player2.skillsToString());
        assertEquals("Weapon{atk=10, skill=null}", player1.weaponToString());
        assertEquals("Weapon{atk=12, skill=AttackedSkill{damage=25, mpUsing=0}}", player2.weaponToString());
    }

    @Test
    public void testPlayerLearnAttackSkill(){
        player1.learnSkill("Fire", skills.get("FireBall"));
        assertEquals("{Fire=AttackedSkill{damage=20, mpUsing=5}}", player1.skillsToString());
    }

    @Test
    public void testPlayerLearnHealingSkill(){
        player1.learnSkill("Heal", skills.get("Healing"));
        assertEquals("{Heal=HealingSkill{healingPoint=10, mpUsing=20}}", player1.skillsToString());
    }

    @Test
    public void testPlayer1AttackPlayer2(){
        player1.attack(player2);
        assertEquals("Status{hp: 100/100, mp: 20/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 140/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testPlayerIsAttacked(){
        player1.isAttacked(10);
        assertEquals("Status{hp: 90/100, mp: 20/20, atk=10}", player1.statusToString());
    }

    @Test
    public void testRegenerateHp(){
        player1.isAttacked(20);
        player1.regenerateHp(10);
        assertEquals("Status{hp: 90/100, mp: 20/20, atk=10}", player1.statusToString());
    }

    @Test
    public void testUsingSkill(){
        player1.learnSkill("Fire", skills.get("FireBall"));
        player1.usingSkillTo(player2, "Fire");
        assertEquals("Status{hp: 100/100, mp: 15/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 140/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testUsingSkillThatPlayerDontHave(){
        player1.usingSkillTo(player2, "Thunder");
        assertEquals("Status{hp: 100/100, mp: 20/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 160/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testCannotUseSkillWhenRunOutOfMp(){
        player1.learnSkill("Fire", skills.get("FireBall"));
        player1.usingSkillTo(player2, "Fire");
        player1.usingSkillTo(player2, "Fire");
        player1.usingSkillTo(player2, "Fire");
        player1.usingSkillTo(player2, "Fire");
        player1.usingSkillTo(player2, "Fire");
        assertEquals("Status{hp: 100/100, mp: 0/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 80/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testUsingWeaponSkill(){
        player2.usingWeaponSkillTo(player1);
        assertEquals("Status{hp: 75/100, mp: 20/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 160/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testUsingWeaponSkillThatWeaponDontHave(){
        player1.usingWeaponSkillTo(player2);
        assertEquals("Status{hp: 100/100, mp: 20/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 160/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testCannotUseWeaponSkillWhenRunOutOfMp(){
        player1.setWeapon(weapons.get("SwordOfHealing"));
        player1.isAttacked(50);
        player1.usingWeaponSkillTo(player1);
        player1.usingWeaponSkillTo(player1);
        player1.usingWeaponSkillTo(player1);
        player1.usingWeaponSkillTo(player1);
        player1.usingWeaponSkillTo(player1);
        player1.usingWeaponSkillTo(player1);
        assertEquals("Status{hp: 70/100, mp: 0/20, atk=10}", player1.statusToString());
    }

    @Test
    public void testSetWeapon(){
        player2.setWeapon(weapons.get("LongSword"));
        assertEquals("Weapon{atk=10, skill=null}", player2.weaponToString());
    }

    @Test
    public void testWhileDeadCannotAttack(){
        player1.isAttacked(1000);
        player1.attack(player2);
        assertEquals("Status{hp: 0/100, mp: 20/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 160/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testWhileDeadCannotLearnSkill(){
        player1.isAttacked(1000);
        player1.learnSkill("Fire", skills.get("FireBall"));
        assertEquals("{}", player1.skillsToString());
    }

    @Test
    public void testWhileDeadCannotRegenMp(){
        player1.learnSkill("Fire",skills.get("FireBall"));
        player1.usingSkillTo(player1, "Fire");
        player1.isAttacked(1000);
        player1.regenerateMp(100);
        assertEquals("Status{hp: 0/100, mp: 15/20, atk=10}", player1.statusToString());
    }

    @Test
    public void testWhileDeadCannotUsingSkill(){
        player1.learnSkill("Fire",skills.get("FireBall"));
        player1.isAttacked(1000);
        player1.usingSkillTo(player1, "Fire");
        assertEquals("Status{hp: 0/100, mp: 20/20, atk=10}", player1.statusToString());
    }

    @Test
    public void testWhileDeadCannotUsingWeaponSkill(){
        player1.setWeapon(weapons.get("ThunderSword"));
        player1.isAttacked(1000);
        player1.usingWeaponSkillTo(player2);
        assertEquals("Status{hp: 0/100, mp: 20/20, atk=10}", player1.statusToString());
        assertEquals("Status{hp: 160/160, mp: 40/40, atk=2}", player2.statusToString());
    }

    @Test
    public void testWhileDeadCannotSetWeapon(){
        player1.isAttacked(1000);
        player1.setWeapon(weapons.get("ThunderSword"));
        assertEquals("Weapon{atk=10, skill=null}", player1.weaponToString());

    }
}