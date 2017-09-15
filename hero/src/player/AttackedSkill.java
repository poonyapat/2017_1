package player;

public class AttackedSkill extends Skill{
    private int damage;

    public AttackedSkill(int mpUsing, int damage) {
        super(mpUsing);
        this.damage = damage;
    }

    @Override
    protected void active(Player target){
        target.isAttacked(damage);
    }

    @Override
    public String toString() {
        return "AttackedSkill{" +
                "damage=" + damage +
                ", mpUsing=" + mpUsing +
                '}';
    }
}
