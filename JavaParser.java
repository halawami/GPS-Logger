import java.io.File;
import java.util.Scanner;

public class Main {

                public enum State {
                                TIME, ALT, COORD, SATS
                }

                public static void main(String[] args) throws Exception {
                                new Main().init();
                }

                String alt = "0";

                private int tmp = 0;

                private State currentState;
                private int stage = 0;

                public void init() throws Exception {

                                Scanner scanner = new Scanner(new File("inout/data.txt"));

                                while (scanner.hasNextLine()) {

                                                String s = scanner.nextLine();

                                                switch (s) {
                                                case "New Data Point":
                                                                System.out.print("$GPGGA,");
                                                                break;
                                                case "Time:":
                                                                currentState = State.TIME;
                                                                stage = 0;
                                                                break;
                                                case "Altitude:":
                                                                currentState = State.ALT;
                                                                stage = 0;
                                                                break;
                                                case "Coordinates:":
                                                                currentState = State.COORD;
                                                                stage = 0;
                                                                break;
                                                case "Satellites:":
                                                                currentState = State.SATS;
                                                                stage = 0;
                                                                break;
                                                default:
                                                                switch (currentState) {
                                                                case TIME:
                                                                                switch (stage) {
                                                                                case 0:
                                                                                case 1:
                                                                                                System.out.format("%02d", Integer.parseInt(s));
                                                                                                break;
                                                                                case 2:
                                                                                                System.out.print(s + ".000,");
                                                                                                break;
                                                                                }
                                                                                break;
                                                                case ALT:
                                                                                switch (stage) {
                                                                                case 0:
                                                                                                tmp = Integer.parseInt(s) << 8;
                                                                                                break;
                                                                                case 1:
                                                                                                tmp |= Integer.parseInt(s);
                                                                                                alt = tmp + ".";
                                                                                                break;
                                                                                case 2:
                                                                                                alt += s;
                                                                                                break;
                                                                                }
                                                                                break;
                                                                case COORD:
                                                                                switch (stage) {
                                                                                case 0:
                                                                                case 2:
                                                                                case 5:
                                                                                case 7:
                                                                                                tmp = Integer.parseInt(s) << 8;
                                                                                                break;
                                                                                case 1:
                                                                                                tmp |= Integer.parseInt(s);
                                                                                                System.out.print(tmp + ".");
                                                                                                break;
                                                                                case 6:
                                                                                                tmp |= Integer.parseInt(s);
                                                                                                System.out.format("%05d.", tmp);
                                                                                                break;
                                                                                case 3:
                                                                                case 8:
                                                                                                tmp |= Integer.parseInt(s);
                                                                                                System.out.print(tmp + ",");
                                                                                                break;
                                                                                case 4:
                                                                                                System.out.print(Character.toString((char) Integer.parseInt(s)) + ",");
                                                                                                break;
                                                                                case 9:
                                                                                                System.out.print(Character.toString((char) Integer.parseInt(s)) + ",1,");
                                                                                                break;
                                                                                }
                                                                                break;
                                                                case SATS:
                                                                                System.out.println(s + ",1.60," + alt + ",M,-33.8,M,,*60");
                                                                                break;
                                                                }
                                                                stage++;
                                                }

                                }

                }

}
