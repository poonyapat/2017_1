package player;

public class Weapon {
    private int atk;
    private boolean skillAvailable;
    private Skill skill;

    public Weapon(int atk, Skill skill) {
        this.atk = atk;
        this.skillAvailable = true;
        this.skill = skill;
    }

    public Weapon(int atk) {
        this.atk = atk;
        this.skillAvailable = false;
    }

    protected int getAtk() {
        return atk;
    }

    protected boolean isSkillAvailable() {
        return skillAvailable;
    }

    protected String skillString(){
        if (skill == null){
            return "";
        }
        return skill.toString();
    }

    protected void weaponSkillActive(Player player){
        if (skillAvailable)
            skill.active(player);
    }

    protected int getWeaponSkillMpUsing(){
        return skill.getMpUsing();
    }

    @Override
    public String toString() {
        return "Weapon{" +
                "atk=" + atk +
                ", skill=" + skill +
                '}';
    }
}
