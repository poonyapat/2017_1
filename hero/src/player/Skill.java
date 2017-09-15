package player;

public abstract class Skill {
    protected int mpUsing;

    public Skill(int mpUsing) {
        this.mpUsing = mpUsing;
    }

    protected int getMpUsing() {
        return mpUsing;
    }

    protected abstract void active(Player target);
}
