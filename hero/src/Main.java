import player.*;

import java.util.HashMap;

public class Main {
    public static void main(String[] args) {
        Player playerA, playerB;

        HashMap<String, Skill> skills = new HashMap<>();
        skills.put("FireBall", new AttackedSkill(5, 20));
        skills.put("ThunderBolt", new AttackedSkill(10, 40));
        skills.put("Healing", new HealingSkill(20, 10));
        skills.put("GreatHealing", new HealingSkill(50, 30));

        HashMap<String, Weapon> weapons = new HashMap<>();
        weapons.put("LongSword", new Weapon(10));
        weapons.put("ShortSword", new Weapon(5));
        weapons.put("knife", new Weapon(3));
        weapons.put("ThunderSword", new Weapon(12, new AttackedSkill(0, 25)));
        weapons.put("SwordOfHealing", new Weapon(10, new HealingSkill(5, 5)));


        playerA = new Player(new Status(100,20,10),
                weapons.get("LongSword"));
        playerB = new Player(new Status(160, 40, 2),
                weapons.get("ThunderSword"));

//        playerA.learnSkill("Fire", skills.get("FireBall"));
//        playerA.learnSkill("TB", skills.get("ThunderBolt"));

        playerA.usingSkillTo(playerB, "Fire");
        playerA.usingSkillTo(playerB, "TB");

        playerA.setWeapon(weapons.get("SwordOfHealing"));
        playerA.usingWeaponSkillTo(playerB);

        playerB.usingWeaponSkillTo(playerA);
        playerB.attack(playerA);

        playerA.regenerateMp(20);

        System.out.println(playerA.toString());
        System.out.println(playerB.toString());
    }
}
