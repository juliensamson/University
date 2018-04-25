/**
 *      Author: Julien Samson
 *      Course: BIE-PSI Computer Network
 *
 *      HOMEWORK 1 TCP
 * **/


package robot;

import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

public class Robot {

    /**     Coordinate class for the Robot's position in the city   **/
    private class Coordinate {
        private int x;
        private int y;

        private void setX(int x) {
            this.x = x;
        }

        private int getX() {
            return x;
        }

        private void setY(int y) {
            this.y = y;
        }

        private int getY() {
            return y;
        }
    }

    /**     Direction enum for the Robot's orientation in the city  **/
    public enum Direction {
        NORTH(1), EAST(2), SOUTH(3), WEST(4);
        private int code;

        //  Assign a code to each direction
        Direction(int code) {
            this.code = code;
        }

        //  Access the code of a direction
        private int getCode() {
            return this.code;
        }

        //  Assign a random direction to the Robot
        private static Robot.Direction getRandDirection() {
            Random random = new Random();
            return values()[random.nextInt(values().length)];
        }

        //  Turn the robot left in the city
        private static Robot.Direction turnLeft(int currentValue) {
            return values()[currentValue - 1];
        }
    }

    /**     Information about the City    **/
    private static final int cityLength = 37;
    private static final int cityHeigth = 37;

    /**     Actor       **/
    private static final String ROBOTNAME = "Astro Le Petit Robot";
    private static final String SERVER = "SERVER";
    private static final String CLIENT = "CLIENT";

    /**     Commands for the Robot       **/
    private static final String STEP = "STEP";
    private static final String LEFT = "LEFT";
    private static final String PICKUP = "PICK UP";
    private static final String REPAIR = "REPAIR";

    /**     System variables  **/
    private static String host;
    private static int port;
    private static Socket cSocket, sSocket;
    private static OutputStreamWriter cOSW, sOSW;
    private static InputStreamReader cISR, sISR;
    private static int waitTime = 400;

    /**     Client variables    **/
    private String mClientRobotName;
    private int stepTaken;
    private int mClientRepairBlock;
    private int mClientRobotAnswerCode;
    private Coordinate mClientCurrCoordinate;
    private Coordinate mClientPrevCoordinate;
    private Direction  mClientDirection;
    private String mSecretMessage;

    /**     Robot(Server) variables     **/
    private int iteration;
    private String commandClientRobotName;
    private Coordinate mRobotPosition;
    private Direction mRobotDirection;
    private int mRobotFailedBlock;
    private boolean mRobotBlockFailure = false;
    private String mRobotCommand;
    private int mRobotRepairBlock;

    public static void main(String[] input) {

        System.out.println(" 2018 Julien Samson");
        System.out.println(" Robot TCP - Homework 1\n\n");


        System.out.println(" Start server by: java -jar Robot.jar <port>");
        System.out.println(" Start client by  java -jar Robot.jar <server> <port>");
        System.out.println(" Start server and client by java -jar Robot.jar <server port> <server> <client port>\n\n");


        switch (input.length) {
            case 1:
                port = Integer.parseInt(input[0]);
                startRobot();
                break;
            case 2:
                host = input[0];
                port = Integer.parseInt(input[1]);
                startClient();
                break;
            case 3:
                int sPort = Integer.parseInt(input[0]);
                host = input[1];
                port = Integer.parseInt(input[2]);
                if(sPort != port) {
                    System.out.println("The port of client does not correspond to the server");
                } else {
                    startRobot();
                    startClient();
                }
                break;
        }
    }

    private static void startClient() {
        (new Thread() {
            @Override
            public void run() {

                Robot robot = new Robot();

                //String host = "baryk.fit.cvut.cz";
                //int port = 3998;
                //String host = "localhost";
                //int port = 3500;

                try {
                    Thread.sleep(waitTime);
                    /**     Obtain an address object of the server  **/
                    InetAddress address = InetAddress.getByName(host);

                    /**     Establish a socket connection  **/
                    cSocket = new Socket(address, port);

                    /**     Instantiate a BufferedOutputStream and BufferedInputStream **/
                    BufferedOutputStream bos = new BufferedOutputStream(cSocket.getOutputStream());
                    cOSW = new OutputStreamWriter(bos);
                    BufferedInputStream bis = new BufferedInputStream(cSocket.getInputStream());
                    cISR = new InputStreamReader(bis);

                    /**     Read message from the server and extract the name from the greeting     **/
                    robot.setRobotName(robot.readMessage(cISR,CLIENT));

                    /**     Send first command. I choose LEFT because we get the coordinate immediately     **/
                    robot.sendMessage(cOSW, CLIENT, LEFT);

                    while (!cSocket.isClosed()) {
                        //  Read the Robot's answer
                        String answer = robot.readMessage(cISR, CLIENT);
                        //  Extract the answer code
                        robot.setRobotAnswerCode(answer);
                        //  Check the robot's answer and choose what to do next.
                        robot.checkRobotAnswer(answer);

                        Thread.sleep(waitTime);
                    }

                } catch(IOException f) {
                    System.out.println("IOException: " + f);
                }
                catch(Exception g) {
                    System.out.println("Exception: " + g);
                }
            }
        }).start();
    }

    private static void startRobot() {
        (new Thread() {
            @Override
            public void run() {
                Robot robot = new Robot();

                /**     Initialize robot initial position, direction, and the city panel    **/
                robot.initRobotPosition();
                robot.setRobotDirection(Direction.getRandDirection());

                String greeting = " Hello you! My name is " + ROBOTNAME + ".";

                try {
                    ServerSocket ss = new ServerSocket(port);
                    sSocket = ss.accept();

                    /**     Instantiate a BufferedOutputStream and BufferedInputStream **/
                    BufferedOutputStream bos = new BufferedOutputStream(sSocket.getOutputStream());
                    sOSW = new OutputStreamWriter(bos);
                    BufferedInputStream bis = new BufferedInputStream(sSocket.getInputStream());
                    sISR = new InputStreamReader(bis);

                    /**     Send greeting to Client     **/
                    robot.sendMessage(sOSW, SERVER, "210" + greeting + "\r\n");

                    while(!sSocket.isClosed()) {
                        //  Check the client message and choose what to do next
                        robot.checkClientCommand();

                        Thread.sleep(waitTime);
                    }

                } catch (IOException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    /******************
     *                *
     * SYSTEM METHODS *
     *                *
     ******************/

    /**     Send message between thread     **/
    private void sendMessage(OutputStreamWriter osw, String actor, String message) {

        try {
            String messageToSend;
            switch (actor) {
                case CLIENT:
                    messageToSend = getRobotName() + " " + message + "\r\n";
                    System.out.printf(messageToSend);
                    if(getCurrCoordinate() != null && getDirection() != null)
                        System.out.println("Position (" + getCurrCoordinate().getX() + "," + getCurrCoordinate().getY() + ") : Orientation " + getDirection());
                    break;
                case SERVER:
                    messageToSend = message;
                    break;
                default:
                    messageToSend = "";
                    System.out.println("sendMessage: No actor selected");
                    break;
            }
            osw.write(messageToSend);
            osw.flush();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**     Reading InputStream from the Client     **/
    private String readMessage(InputStreamReader isr, String actor) {
        StringBuffer stringBuffer = new StringBuffer();
        try {
            int data;
            //  Read every character and append to string buffer
            while ((data = isr.read()) != 13) {
                stringBuffer.append((char) data);
            }
            if(actor.equals(CLIENT))
                System.out.println(stringBuffer);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        return stringBuffer.toString();
    }

    /**     Close connection        **/
    private void closeConnection(Socket connection) {
        try {
            connection.close();
        }catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**     Set the OK message to avoid to rewrite it every time     **/
    private String okMessage() {
        return " OK (" + getRobotPosition().getX() + "," + getRobotPosition().getY() + ")\r\n";
    }

    /********************************
     *                              *
     * CLIENT ROBOT RELATED METHODS *
     *                              *
     ********************************/

    /**     Check if the Client must end the connection     **/
    private void checkRobotAnswer(String answer) {

        boolean endConnection = ( getRobotAnswerCode() == 530 || getRobotAnswerCode() == 550
                || getRobotAnswerCode() == 571 || getRobotAnswerCode() == 572);

        //  Check if the Client must end the connection
        if(!endConnection)
        {
            switch (getRobotAnswerCode()) {
                case 210:
                    setRobotName(answer);
                    break;
                case 240:
                    setCurrCoordinate(answer);
                    selectClientCommand();
                    setPrevCoordinate(getCurrCoordinate());
                    break;
                case 260:
                    setSecretMessage(answer);
                    System.out.println("The secret message is " + getSecretMessage());
                    closeConnection(cSocket);
                    break;
                case 500:   //Unknown. Continue connection.
                    sendMessage(cOSW, CLIENT, STEP);
                    break;
                case 580:   //Failure of block X. REPAIR
                    setRepairBlock(answer);
                    sendMessage(cOSW,CLIENT, REPAIR + " " + getRepairBlock());
                    stepTaken = 0;
                    break;
                default:
                    closeConnection(cSocket);
                    break;
            }
        } else {
            closeConnection(cSocket);
        }
    }

    /**     Command to move the robot in direction of the flag      **/
    private void moveRobotToFlag() {

        /**    Example:
         *     If the robot face NORTH and the value of Y >= 0,
         *     the robot must Turn left (to the EAST) because if he kept moving NORTH,
         *     it would fall off the board. Then, next iteration, the method check if
         *     the Robot should move EAST (if X < 0) or Turn left to get closer to (0,0).
         **/

        if(!checkForIncomingCrash()) {
            switch (getDirection()) {
                case NORTH:
                    if (getCurrCoordinate().getY() >= 0) {
                        sendMessage(cOSW, CLIENT, LEFT);
                        setDirection(Direction.WEST);
                    } else
                        sendMessage(cOSW, CLIENT, STEP);
                    break;
                case SOUTH:
                    if (getCurrCoordinate().getY() <= 0) {
                        sendMessage(cOSW, CLIENT, LEFT);
                        setDirection(Direction.EAST);
                    } else
                        sendMessage(cOSW, CLIENT, STEP);
                    break;
                case EAST:
                    if (getCurrCoordinate().getX() >= 0) {
                        sendMessage(cOSW, CLIENT, LEFT);
                        setDirection(Direction.NORTH);
                    } else
                        sendMessage(cOSW, CLIENT, STEP);
                    break;
                case WEST:
                    if (getCurrCoordinate().getX() <= 0) {
                        sendMessage(cOSW, CLIENT, LEFT);
                        setDirection(Direction.SOUTH);
                    } else
                        sendMessage(cOSW, CLIENT, STEP);
                    break;
                default:
                    sendMessage(cOSW, CLIENT, STEP);
                    System.out.printf("Direction Unknown\r\n");
                    break;
            }
        }
    }

    /**     Decide which is the next action to take. Pick up, step, or left     **/
    private void selectClientCommand() {

        Coordinate currCoordinate = getCurrCoordinate();

        /**     If coordinate are (0,0) the robot pickup the flag       **/
        if (currCoordinate.getX() == 0 && currCoordinate.getY() == 0) {
            sendMessage(cOSW, CLIENT, PICKUP);
        }
        else {
            /**     The robot need to take at least 2 step in row before getting his direction  **/
            if(stepTaken >= 1) {        //Resolve NullPointer Exception
                extractDirectionFromCoordinate(getPrevCoordinate(), getCurrCoordinate());
                moveRobotToFlag();
            } else {
                sendMessage(cOSW, CLIENT, STEP);
                stepTaken++;
            }
        }
    }

    /**     Check if the next step make the robot crash     **/
    private boolean checkForIncomingCrash() {

        if(getDirection() != null && getCurrCoordinate() != null)
        {
            Coordinate coord = getCurrCoordinate();
            Direction direct = getDirection();

            if (coord.getX() == 18 && direct == Direction.EAST) {
                sendMessage(cOSW, CLIENT, LEFT);
                setDirection(Direction.NORTH);
                return true;
            } else if (coord.getX() == -18 && direct == Direction.WEST) {
                sendMessage(cOSW, CLIENT, LEFT);
                setDirection(Direction.SOUTH);
                return true;
            } else if (coord.getY() == 18 && direct == Direction.NORTH) {
                sendMessage(cOSW, CLIENT, LEFT);
                setDirection(Direction.WEST);
                return true;
            } else if (coord.getY() == -18 && direct == Direction.SOUTH) {
                sendMessage(cOSW, CLIENT, LEFT);
                setDirection(Direction.EAST);
                return true;
            } else
                return false;
        } else
            return false;
    }

    /********************************
     *                              *
     * SERVER ROBOT RELATED METHODS *
     *                              *
     ********************************/

    /**     Check the command of the client and act appropriately **/
    private void checkClientCommand() {

        //  Read the client command
        String command = readMessage(sISR, SERVER);
        //  Extract the command from the message
        setClientCommand(command);

        if (iteration < 1) {
            int nameEndAt = command.lastIndexOf(" ");
            commandClientRobotName = command.substring(0, nameEndAt);
        }

        if (ROBOTNAME.equals(commandClientRobotName)) {
            switch (getClientCommand()) {
                case STEP:
                    //  Check if the robot has a block failure. If so, he is destroyed.
                    if (!getBlockFailure())
                        Step();
                    else {
                        sendMessage(sOSW, SERVER, 572 + " ROBOT FELLS TO PIECES\r\n");
                        closeConnection(sSocket);
                    }
                    break;
                case LEFT:
                    Left();
                    break;
                case REPAIR:
                    Repair();
                    break;
                case PICKUP:
                    PickUp();
                    break;
                default:
                    sendMessage(sOSW, SERVER, 500 + " UNKNOWN COMMAND\r\n");
                    break;
            }
        } else {
            sendMessage(sOSW, SERVER, 500 + " UNKNOWN COMMAND\r\n");
            closeConnection(sSocket);
        }
        iteration++;
    }

    /**     Initialize the robot's position in the city     **/
    private void initRobotPosition() {
        //  Initialize a random initiale position to the robot
        Random r = new Random();
        Coordinate coord = new Coordinate();

        coord.setX((r.nextInt((cityLength - 2) + 1)) - 18);
        coord.setY(18 - (r.nextInt((cityHeigth - 2) + 1)));
        setRobotPosition(coord);
    }

    /**     Make the robot take a Step      **/
    private void Step() {

        //  Check if the Robot's Sytstem fail
        if(!robotSystemFail()) {
            //  If false, move robot.
            moveRobot();

            //  Check if robot is out of the city
            if (robotOutOfBoundary()) {
                //  If true, the Robot crashes and the connection is closed
                sendMessage(sOSW, SERVER, 530 + " CRASHED\r\n");
                closeConnection(sSocket);
            }
            else {
                //  If false, send OK message to Client
                sendMessage(sOSW,SERVER, 240 + okMessage());
            }
        } else {
            //  If true, check Client Command again to determine the next action
            checkClientCommand();
        }
    }

    /**     Turn the robot to the left      **/
    private void Left() {
        //  Get current direction of the robot
        int currDirection = getRobotDirection().getCode();

        //  Turn left and set new robot direction
        if(currDirection == 1)
            setRobotDirection(Direction.turnLeft(4));
        else
            setRobotDirection(Direction.turnLeft(currDirection - 1));

        sendMessage(sOSW, SERVER, 240 + okMessage());
    }

    /**     Repair the robot    **/
    private void Repair() {

        //  Check if Client's Repair Block equal to the Robot's Fail Block
        if(getFailBlock() == getClientRepairBlock()) {
            //  Set BlockFailure back to false since block is repaired
            setBlockFailure(false);
            sendMessage(sOSW, SERVER, 240 + okMessage());
        }
        else {
            //  Different repair block is called. Send answer 571 to Client
            sendMessage(sOSW, SERVER, 571 + " THIS BLOCK IS OK\r\n");
            closeConnection(sSocket);
        }
    }

    /**     Pick Up the secret message      **/
    private void PickUp() {
        String secretText = " You've got mail!";
        //  Get the current position of th robot
        int x = getRobotPosition().getX();
        int y = getRobotPosition().getY();

        //  Check Robot's position equal (18,18) ~ (0,0) for the Client
        if (x == 0 && y == 0) {
            //  Send the secret text if Client is at (0,0)
            sendMessage(sOSW, SERVER, 260 + " SUCCESS " + secretText + "\r\n");
            try {
                Thread.sleep(1000);
                closeConnection(sSocket);
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            //  Send answer 550 to Client and close connection
            sendMessage(sOSW, SERVER, 550 + " CANNOT PICK UP THE SIGN\r\n");
            closeConnection(sSocket);
        }
    }

    /**     Check for Robot's System Failure        **/
    private boolean robotSystemFail() {

        /*
         *       Generate a random number between 1 and 30.
         *       If the number is under 10 (1-9) the robot's
         *       system fail. ~ (1 on 3 chances to fail)
         */

        Random rand = new Random();
        int max = 30, min = 1;
        int failBlock = rand.nextInt(((max - min) + 1) + min);

        //  Check if the Robot's System failed
        if(failBlock != 0 && failBlock < 10) {
            //  Set the what Robot's Block failed
            setFailBlock(failBlock);
            //  Set block failure to true so that he knows to repair
            setBlockFailure(true);
            //  Send answer 580 with the failed block to Client
            sendMessage(sOSW, SERVER,580 +  " FAILURE OF BLOCK " + failBlock + "\r\n");
            return true;
        }
        else {
            //  System didn't fail so set Failure to false
            setBlockFailure(false);
            return false;
        }
    }

    /**     Method in charge of moving the robot    **/
    private void moveRobot() {
        //  Get current coordinate
        Coordinate currCoord = getRobotPosition();
        int x = currCoord.getX();
        int y = currCoord.getY();

        //  Check what direction the robot is facing
        //  Add 1 (or -1) to X (or Y) depending the faced direction
        switch (getRobotDirection()) {
            case NORTH:
                currCoord.setY(y + 1);
                break;
            case SOUTH:
                currCoord.setY(y - 1);
                break;
            case EAST:
                currCoord.setX(x + 1);
                break;
            case WEST:
                currCoord.setX(x - 1);
                break;
            default:
                break;
        }
    }

    /**     Check if the Robot moved outside the city's boundaries      **/
    private boolean robotOutOfBoundary() {
        //  Set condition which corresponds to the boundary of the city
        boolean overX = getRobotPosition().getX() < -18 || getRobotPosition().getX() > 18;
        boolean overY = getRobotPosition().getY() < -18 || getRobotPosition().getY() > 18;
        return (overX || overY);
    }

    /****************************
     *                          *
     * CLIENT SETTERs & GETTERs *
     *                          *
     ****************************/

    /**     Get the name of the robot       **/
    private void setRobotName(String message) {

        //  Set markers where we can find the name
        String myNameIs = "My name is ";
        String dot  = ".";

        //  Get the whole string after "My name is"
        int nameStartAt = message.indexOf(myNameIs) + myNameIs.length();
        int nameEndAt   = message.lastIndexOf(dot) ;                  //  message.indexOf(dot, nameStartAt);
        String robotName = message.substring(nameStartAt, nameEndAt);

        //  Check the number of occurrence of "." in the string.
        int occurrence = 0;
        for (int i = -1; (i = robotName.indexOf(dot, i + 1)) != -1; i++)
            occurrence = i;

        //  If the number of occurrence > 1, loop until there is only 1 "." and assigne the name
        if(occurrence > 1) {
            nameStartAt = 0;
            nameEndAt = robotName.indexOf(dot);
            robotName = robotName.substring(nameStartAt, nameEndAt);
        }
        this.mClientRobotName = robotName;
    }

    private String getRobotName() {
        return this.mClientRobotName;
    }

    /**     Get Robot's answer code     **/
    private void setRobotAnswerCode(String message) {
        int index = message.indexOf(" ");
        this.mClientRobotAnswerCode = Integer.parseInt(message.substring(index - 3, index));
    }

    private int getRobotAnswerCode() {
        return this.mClientRobotAnswerCode;
    }

    /**     Get the current coordinate of the robot     **/
    private void setCurrCoordinate(String message) {

        this.mClientCurrCoordinate = new Coordinate();

        /**     Transform server answer to a string with number and minus only      **/
        String numberOnly= message.replaceAll("[^0-9 /-]", " ");

        /**     Get the string which contain the coordinate     **/
        int index = message.indexOf(" ") + 5;
        String sCoord = numberOnly.substring(index);

        /**     Set the coordinate for X and Y      **/
        int indexY = sCoord.indexOf(" ");
        int length = sCoord.length();
        this.mClientCurrCoordinate.setX(Integer.parseInt(sCoord.substring(0,indexY)));
        this.mClientCurrCoordinate.setY(Integer.parseInt(sCoord.substring(indexY + 1, length - 1)));
    }

    private Coordinate getCurrCoordinate() {
        return this.mClientCurrCoordinate;
    }

    /**     Get the previous coordinate of the robot     **/
    private void setPrevCoordinate(Coordinate coordinate) {
        this.mClientPrevCoordinate = coordinate;
    }

    private Coordinate getPrevCoordinate() {
        return this.mClientPrevCoordinate;
    }

    /**     Get the direction of the robot      **/
    private void extractDirectionFromCoordinate(Coordinate prev, Coordinate curr) {

        //  Get the difference between previous and current coordinate
        int diffX = prev.getX() - curr.getX();
        int diffY = prev.getY() - curr.getY();

        if(diffX != 0)
        {
            if (diffX == 1)
                this.mClientDirection = Direction.WEST;
            else
                this.mClientDirection = Direction.EAST;
        }
        else if (diffY != 0)
        {
            if(diffY == 1)
                this.mClientDirection = Direction.SOUTH;
            else
                this.mClientDirection = Direction.NORTH;
        }
    }

    private void setDirection(Direction direction) {
        this.mClientDirection = direction;
    }

    private Direction getDirection() {
        return this.mClientDirection;
    }

    /**     Get the block which need to be repaired     **/
    private void setRepairBlock(String message) {
        int length = message.length();
        this.mClientRepairBlock = Integer.parseInt(message.substring(length - 1));
    }

    private int getRepairBlock() {
        return this.mClientRepairBlock;
    }

    /**   Get the secret message  **/
    private void setSecretMessage(String message) {
        int index = message.indexOf(" ");
        this.mSecretMessage = message.substring(index + 9);
    }

    private String getSecretMessage() {
        return this.mSecretMessage;
    }
    /****************************
     *                          *
     * SERVER SETTERs & GETTERs *
     *                          *
     ****************************/

    /**     Set the robot's position in the city      **/
    private void setRobotPosition(Coordinate coordinate) {
        this.mRobotPosition = coordinate;
    }

    private Coordinate getRobotPosition() {
        return this.mRobotPosition;
    }

    /**     Set the robot's direction         **/
    private void setRobotDirection(Direction direction) {
        this.mRobotDirection = direction;
    }

    private Direction getRobotDirection() {
        return this.mRobotDirection;
    }


    /**     Get client command from message     **/
    private void setClientCommand(String message) {
        String repair = "REPAIR", pickup = "PICK UP";

        int commandStartAt = message.lastIndexOf(" ");
        int messageLength = message.length();

        if(message.contains(repair)) {
            this.mRobotRepairBlock = Integer.parseInt(message.substring(commandStartAt + 1, messageLength));
            this.mRobotCommand = message.substring(message.indexOf(repair), message.indexOf(repair) + repair.length());
        } else if (message.contains(pickup)){
            this.mRobotCommand = message.substring(message.indexOf(pickup), message.indexOf(pickup) + pickup.length());
        }
        else {
            this.mRobotCommand = message.substring(commandStartAt + 1, messageLength);
        }

    }

    private String getClientCommand() {
        return this.mRobotCommand;
    }

    private int getClientRepairBlock() {
        return this.mRobotRepairBlock;
    }

    /**     Check if a block has failed or not      **/
    private void setBlockFailure(boolean isSystemFail) {
        this.mRobotBlockFailure = isSystemFail;
    }

    private boolean getBlockFailure() {
        return this.mRobotBlockFailure;
    }

    /**     Check what block has failed      **/
    private void setFailBlock(int failBlock) {
        this.mRobotFailedBlock = failBlock;
    }

    private int getFailBlock() {
        return this.mRobotFailedBlock;
    }

}
