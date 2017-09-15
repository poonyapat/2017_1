package player;

public class HealingSkill extends Skill{
    private int healingPoint;

    public HealingSkill(int mpUsing, int healingPoint) {
        super(mpUsing);
        this.healingPoint = healingPoint;
    }

    @Override
    protected void active(Player target){
        target.regenerateHp(healingPoint);
    }

    @Override
    public String toString() {
        return "HealingSkill{" +
                "healingPoint=" + healingPoint +
                ", mpUsing=" + mpUsing +
                '}';
    }
}
