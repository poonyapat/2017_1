package player;

public class Status {

    private int maxHp, maxMp, hp, mp, atk;

    private boolean isDead;

    public Status(int maxHp, int maxMp, int atk) {
        this.maxHp = maxHp;
        this.maxMp = maxMp;
        this.hp = maxHp;
        this.mp = maxMp;
        this.atk = atk;
        isDead = false;
    }

    protected void decreaseHp(int damage){
        hp -= damage;
        if (hp <= 0) {
            hp = 0;
            isDead = true;
        }
    }

    protected boolean isDead() {
        return isDead;
    }

    protected boolean decreaseMp(int mpUsing){
        if (mp < mpUsing){
            return false;
        }
        mp -= mpUsing;
        return true;
    }

    protected void regenerateHp(int hp){
        this.hp += hp;
        if (this.hp > maxHp){
            this.hp = maxHp;
        }
    }

    protected void regenerateMp(int mp){
        this.mp += mp;
        if (this.mp > maxMp){
            this.mp = maxMp;
        }
    }

    protected int getAtk() {
        return atk;
    }

    @Override
    public String toString() {
        return "Status{" +
                "hp: " + hp + "/" + maxHp +
                ", mp: " + mp + "/" + maxMp +
                ", atk=" + atk + "}";
    }
}
