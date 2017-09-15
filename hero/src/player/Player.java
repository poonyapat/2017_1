package player;

import java.util.HashMap;

public class Player {
    private Status status;
    private HashMap<String, Skill> skills = new HashMap<>();
    private Weapon weapon;

    public Player(Status status, Weapon weapon) {
        this.status = status;
        this.skills = new HashMap<>();
        this.weapon = weapon;
    }

    public void learnSkill(String skillName,Skill skill){
        if (status.isDead())
            return;
        skills.put(skillName, skill);
    }

    public void attack(Player target){
        if (status.isDead())
            return;
        target.isAttacked(status.getAtk() + weapon.getAtk());
    }

    public void isAttacked(int damage){
        status.decreaseHp(damage);
    }

    protected void regenerateHp(int hp){
        status.regenerateHp(hp);
    }

    public void regenerateMp(int mp) {
        if (status.isDead())
            return;
        status.regenerateMp(mp);
    }

    public void usingSkillTo(Player target, String skillName){
        if (status.isDead())
            return;
        if (skills.containsKey(skillName) == false)
            return;
        if (!status.decreaseMp(skills.get(skillName).getMpUsing()))
            return;
        skills.get(skillName).active(target);
    }

    public void usingWeaponSkillTo(Player target){
        if (status.isDead())
            return;
        if (!weapon.isSkillAvailable())
            return;
        if (!status.decreaseMp(weapon.getWeaponSkillMpUsing()))
            return;
        weapon.weaponSkillActive(target);
    }

    public void setWeapon(Weapon weapon) {
        if (status.isDead())
            return;
        this.weapon = weapon;
    }

    public String statusToString(){
        return status.toString();
    }

    public String skillsToString(){
        return skills.toString();
    }

    public String weaponToString(){
        return weapon.toString();
    }

    @Override
    public String toString() {
        return "Player : " + "\n" +
                "status=" + status + "\n" +
                ", skills=" + skills + "\n" +
                ", weapon=" + weapon;
    }
}
