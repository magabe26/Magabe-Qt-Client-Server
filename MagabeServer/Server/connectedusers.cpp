/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "connectedusers.h"
#include "debug.h"
#include "security.h"
#include "Helpers/connectionpipe.h"
#include "registeredobjects.h"

/** --------------------------MSessions------------------------------------
 * @brief The MSessions class
 */
class MSessions{

public:

    /** optimize for easy search inside a QList
   * @brief The ConnectedClient class
   */
    class  ConnectedClient{
    public:
        enum class SearchBy{
            UserId,
            MgbSessionId,
            Socket,
            AuthenticationStatus,
            All
        };

        ConnectedClient()
            :searchBy_(SearchBy::All)
            ,userId_(0)
            ,socket_(nullptr)
            ,authenticated_(false)
        {
            mgbSessionId_.clear();
        }

        ConnectedClient(const QString& mgbSessionId,const QSslSocket* socket)
            :searchBy_(SearchBy::All)
            ,userId_(0)
            ,authenticated_(false)
        {
            setMgbSessionId(mgbSessionId);
            setSocket(socket);
        }

        bool isValid(){
            return ((!mgbSessionId_.isEmpty()) && (socket_));
        }

        bool operator == (const ConnectedClient& rhs){

            switch (rhs.searchBy_) {

            case SearchBy::UserId: return (this->userId_ == rhs.userId_);

            case SearchBy::MgbSessionId: return (this->mgbSessionId_ == rhs.mgbSessionId_);

            case SearchBy::Socket :return (this->socket_ == rhs.socket_);

            case SearchBy::AuthenticationStatus : return (this->authenticated_ == rhs.authenticated_);

            case SearchBy::All: return (   (this->userId_ == rhs.userId_)
                                           && (this->mgbSessionId_ == rhs.mgbSessionId_)
                                           && (this->socket_ == rhs.socket_)
                                           && (this->authenticated_ == rhs.authenticated_));

            default:return (   (this->userId_ == rhs.userId_)
                               && (this->mgbSessionId_ == rhs.mgbSessionId_)
                               && (this->socket_ == rhs.socket_)
                               && (this->authenticated_ == rhs.authenticated_));
            }
        }

        bool operator != (const ConnectedClient& rhs){

            switch (rhs.searchBy_) {

            case SearchBy::UserId: return (this->userId_ != rhs.userId_);

            case SearchBy::MgbSessionId: return (this->mgbSessionId_ != rhs.mgbSessionId_);

            case SearchBy::Socket :return (this->socket_ != rhs.socket_);

            case SearchBy::AuthenticationStatus : return (this->authenticated_ != rhs.authenticated_);

            case SearchBy::All: return (   (this->userId_ != rhs.userId_)
                                           || (this->mgbSessionId_ != rhs.mgbSessionId_)
                                           || (this->socket_ != rhs.socket_)
                                           || (this->authenticated_ != rhs.authenticated_));

            default:return (   (this->userId_ != rhs.userId_)
                               || (this->mgbSessionId_ != rhs.mgbSessionId_)
                               || (this->socket_ != rhs.socket_)
                               || (this->authenticated_ != rhs.authenticated_));
            }
        }

        void setSearchBy(const SearchBy &searchBy)
        {
            searchBy_ = searchBy;
        }

        unsigned long userId() const
        {
            return userId_;
        }

        void setUserId(unsigned long userId)
        {
            userId_ = userId;
        }

        QString mgbSessionId() const
        {
            return mgbSessionId_;
        }

        void setMgbSessionId(const QString &mgbSessionId)
        {
            mgbSessionId_ = mgbSessionId;
        }

        void setSocket(const QSslSocket *socket)
        {
            socket_ = const_cast<QSslSocket*>(socket);
        }

        QSslSocket *socket() const
        {
            return socket_;
        }

        bool authenticated() const
        {
            return authenticated_;
        }

        void setAuthenticated(bool authenticated)
        {
            authenticated_ = authenticated;
        }

        SearchBy searchBy() const
        {
            return searchBy_;
        }

    private:
        SearchBy searchBy_;

        unsigned long userId_;
        QString mgbSessionId_;
        QSslSocket* socket_;
        bool authenticated_;
    };


    ~MSessions(){
        closeAllSessions();
    }

    void closeAllSessions(){
        if(connectedClients_.isEmpty()){
            return;
        }
        foreach (ConnectedClient connectedClient, connectedClients_) {
            if(connectedClient.isValid()){
                QSslSocket* socket = const_cast<QSslSocket*>(connectedClient.socket());
                if(socket){//test socket again,Note please do not disconnect signals
                    if(socket->isOpen()){
                        socket->close();
                    }
                    socket->deleteLater();
                }
            }
        }
        connectedClients_.clear();
    }

    QString generateMgbSessionId()
    {
        //UNIQUE constraint failed
        QString str("789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopgrst0123456");
        QString generatedStr;
        srand(QDateTime::currentDateTime().toMSecsSinceEpoch());

        for(int x = 1; x <= 52;x++){
            int i = rand()%str.size();
            generatedStr += str.at(i) ;
        }

        while (containsMgbSessionId(generatedStr)) { //avoid duplicated connection that cause crashes
            // print("CUsers ::mgbSessionId ="+generatedStr+" exists ,generating another one");
            generatedStr = generateMgbSessionId();
            srand(QDateTime::currentDateTime().toMSecsSinceEpoch());
        }

        return generatedStr;
    }

    bool containsMgbSessionId(const QString& mgbSessionId){
        if(connectedClients_.isEmpty()) return false;

        ConnectedClient clientToFind = createSignatureObject(mgbSessionId);
        return connectedClients_.contains(clientToFind);
    }

    bool addNewSession(const QSslSocket* socket,QString& mgbSessionId){
        mgbSessionId.clear();
        if(socket){

            const QString& newSessionId = generateMgbSessionId();

            ConnectedClient connectedClient(newSessionId,socket);
            connectedClient.setSearchBy(ConnectedClient::SearchBy::Socket);

            if(!connectedClients_.contains(connectedClient)){
                connectedClient.setAuthenticated(false);
                connectedClient.setSearchBy(ConnectedClient::SearchBy::All);//reset, not neccessary tho
                connectedClients_.append(connectedClient);
                mgbSessionId = newSessionId;
                return true;
            }

            qWarning() << QString("MSessions::addNewSession | Error ;- Session for Client with "
                                  "socketDescriptor# = %1 already added !").arg(socket->socketDescriptor());

            return false;
        }else{
            qWarning() << QString("MSessions::addNewSession | Error ;- Cant add Session for null socket ");
            return false;
        }
    }

    bool tagAsAuthenticatedSession(const QString& mgbSessionId, unsigned long  userId){
        if(userId == 0){
            qWarning() << "tagAsAuthenticatedSession:: ALERT USER ID  == 0 " ;
        }
        if(mgbSessionId.isEmpty() || (userId == 0)){
            return false;
        }

        if(containsMgbSessionId(mgbSessionId)){
            ConnectedClient clientToFind = createSignatureObject(mgbSessionId);

            const int count = connectedClients_.count(clientToFind);
            if(count > 1){
                qWarning() << QString("MSessions::tagAsAuthenticatedSession | Critical Error can not tag ;- "
                                      "Found more than one (count = %1) Session(s) with the same mgbSessionId = %2 ").arg(count).arg(mgbSessionId);
                return false;
            }else{

                const int index = connectedClients_.indexOf(clientToFind);
                auto connectedClient = connectedClients_.takeAt(index); //remove it
                connectedClient.setUserId(userId);//setUserId
                connectedClient.setAuthenticated(true); //change authentcation status to  true
                //then add it back
                connectedClients_.insert(index,connectedClient);
                return true;
            }

        }else{
            qWarning() << QString("MSessions::tagAsAuthenticatedSession | Error can not tag ;- mgbSessionId = %1 is not a valid ").arg(mgbSessionId);
            return false;
        }

    }

    bool tagAsNonAuthenticatedSession(const QString& mgbSessionId){

        if(containsMgbSessionId(mgbSessionId)){
            ConnectedClient clientToFind = createSignatureObject(mgbSessionId);

            const int count = connectedClients_.count(clientToFind);
            if(count > 1){
                qWarning() << QString("MSessions::tagAsNonAuthenticatedSession| Critical Error can not tag ;- "
                                      "Found more than one (count = %1) Session(s) with the same mgbSessionId = %2 ").arg(count).arg(mgbSessionId);
                return false;
            }else{
                const int index = connectedClients_.indexOf(clientToFind);
                auto connectedClient = connectedClients_.takeAt(index);//remove it
                connectedClient.setAuthenticated(false); //change authentcation status to false
                //then add it back
                connectedClients_.insert(index,connectedClient);
                return true;
            }

        }else{
            qWarning() << QString("MSessions::tagAsNonAuthenticatedSession | Error can not tag ;- mgbSessionId = %1 is not a valid ").arg(mgbSessionId);
            return false;
        }
    }

    int sessionsCount(){
        return connectedClients_.count();
    }

    bool closeSession(const QString& mgbSessionId){
        if((mgbSessionId.isEmpty()) ){
            return false;
        }

        auto signatureObj = createSignatureObject(mgbSessionId);
        bool closed = false;
        int count = 0;
        for(int index = connectedClients_.indexOf(signatureObj);
            index != -1; index = connectedClients_.indexOf(signatureObj)){//i do not increment ,bcoz i am using takeAt

            QSslSocket* socket = const_cast<QSslSocket*>(connectedClients_.takeAt(index).socket());
            if(socket){//note please do not disconnect signals
                if(socket->isOpen()){
                    socket->close();
                }
                socket->deleteLater();
                if(!closed){
                    closed = true;
                }
            }
            count ++;
        }
        if(count > 1){
            qWarning() << QString("MSessions::closeSession| Critical Error "
                                  "Found more than one (count = %1) Session(s) with the same mgbSessionId = %2 ").arg(count).arg(mgbSessionId);
        }
        return closed;
    }

    QList<QString> getUserSessions(unsigned long  userId){
        ConnectedClient signatureObject = createSignatureObject(userId);
        QList<QString> list;

        for(int index  = connectedClients_.indexOf(signatureObject);
            index != -1 ; index = connectedClients_.indexOf(signatureObject,(index+1))){

            auto connectedClient = connectedClients_.at(index);
            if(connectedClient.isValid()){
                list.append(connectedClient.mgbSessionId());
            }else{
                qWarning() << QString("MSessions::getUserSessions | Critcal Error found INVALID connectedClient;THIS IS A BUG ,FIX THIS BUG IMMEDIATRY ");
            }
        }

        return list;
    }

    QList<const QSslSocket*> getSocketsForUserSessions(unsigned long  userId){
        ConnectedClient signatureObject = createSignatureObject(userId);
        QList<const QSslSocket*> list;

        for(int index  = connectedClients_.indexOf(signatureObject);
            index != -1 ; index = connectedClients_.indexOf(signatureObject,(index+1))){

            auto connectedClient = connectedClients_.at(index);
            if(connectedClient.isValid()){
                list.append(connectedClient.socket());
            }else{
                qWarning() << QString("MSessions::getSocketsForUserSessions | Critcal Error found INVALID connectedClient;THIS IS A BUG ,FIX THIS BUG IMMEDIATRY ");
            }
        }

        return list;
    }

    int getUserSessionsCount(unsigned long  userId){
        return getUserSessions(userId).count();
    }

    unsigned long  getUserId(const QString& mgbSessionId){
        return getConnectedClient(mgbSessionId).userId();
    }

    bool userExists(const QString& mgbSessionId){
        return connectedClients_.contains(createSignatureObject(mgbSessionId));
    }

    bool userExists(const QSslSocket *client){
        return connectedClients_.contains(createSignatureObject(client));
    }

    QString getMgbSessionId(const QSslSocket *client){
        return getConnectedClient(client).mgbSessionId();
    }

    const QSslSocket* getSocket(const QString& mgbSessionId){
        if(containsMgbSessionId(mgbSessionId)){
            return getConnectedClient(mgbSessionId).socket();
        }else{
            return nullptr;
        }
    }

    bool isAuthenticated(const QString& mgbSessionId){
        auto conectedClient =  getConnectedClient(mgbSessionId);
        if(conectedClient .isValid()){
            return conectedClient .authenticated();
        }else{
            return false;
        }
    }

    bool hasActiveSessions(unsigned long  userId){
        return connectedClients_.contains(createSignatureObject(userId));
    }

    int authenticedClientsCount(){
        return connectedClients_.count(createSignatureObject(true));
    }

    int unAuthenticatedClientsCount(){
        return connectedClients_.count(createSignatureObject(false));
    }

    QStringList getAllMgbSessionIds() const
    {
        QStringList list;
        foreach (ConnectedClient connectedClient, connectedClients_) {
            if(connectedClient.isValid()){
                list.append(connectedClient.mgbSessionId());
            }else{
                qWarning() << QString("MSessions::getAllMgbSessionIds | Error Found Invalid Connected Client");
            }
        }
        return list;
    }

private:
    QList<ConnectedClient> connectedClients_;

    ConnectedClient createSignatureObject(const QString& mgbSessionId){
        ConnectedClient clientToFind;
        clientToFind.setMgbSessionId(mgbSessionId);
        clientToFind.setSearchBy(ConnectedClient::SearchBy::MgbSessionId);
        return clientToFind;
    }

    ConnectedClient createSignatureObject(const QSslSocket *client){
        ConnectedClient clientToFind;
        clientToFind.setSocket(client);
        clientToFind.setSearchBy(ConnectedClient::SearchBy::Socket);
        return clientToFind;
    }

    ConnectedClient createSignatureObject(unsigned long  userId){
        ConnectedClient clientToFind;
        clientToFind.setUserId(userId);
        clientToFind.setSearchBy(ConnectedClient::SearchBy::UserId);
        return clientToFind;
    }

    ConnectedClient createSignatureObject(bool  authenticationStatus){
        ConnectedClient clientToFind;
        clientToFind.setAuthenticated(authenticationStatus);
        clientToFind.setSearchBy(ConnectedClient::SearchBy::AuthenticationStatus);
        return clientToFind;
    }

    /* there can be only one*/
    ConnectedClient getConnectedClient(const QSslSocket *client){
        if(!client) return ConnectedClient();

        auto connectedClients = getConnectedClients(createSignatureObject(client));
        const int count = connectedClients.count();

        if(count == 0) return ConnectedClient();

        if(count > 1){
            qWarning() << QString("MSessions::getConnectedClient(const QSslSocket *client) | Critcal Error found more than one connected client ( count = %1) "
                                  "with the same socket; socketDescriptor = %2 THIS IS A BUG, FIX THIS BUG IMMEDIATRY = (for now returning the last element in the list)").arg(count).arg(client->socketDescriptor());
        }

        return connectedClients.last();//we are sure the list is not empty{coz if is empty the app will crash}
    }

    /*mgbSessionId is assocciatedwith only one connected client */
    ConnectedClient getConnectedClient(const QString& mgbSessionId){
        if(mgbSessionId.isEmpty()) return ConnectedClient();

        auto connectedClients = getConnectedClients(createSignatureObject(mgbSessionId));
        const int count = connectedClients.count();

        if(count == 0) return ConnectedClient();

        if(count > 1){
            qWarning() << QString("MSessions::getConnectedClient(const QString& mgbSessionId) | Critcal Error found more than one connected client ( count = %1) "
                                  "with the same mgbSessionId = %2 THIS IS A BUG, FIX THIS BUG IMMEDIATRY = (for now returning the last element in the list)").arg(count).arg(mgbSessionId);

        }
        return connectedClients.last();//we are sure the list is not empty{coz if is empty the app will crash}
    }

    /* A user can have as many connected client*/
    QList<ConnectedClient> getConnectedClients(unsigned long  userId){
        return getConnectedClients(createSignatureObject(userId));

    }

    QList<ConnectedClient> getConnectedClients(ConnectedClient signatureObject){
        QList<ConnectedClient> list;
        for(int index  = connectedClients_.indexOf(signatureObject);
            index != -1 ; index = connectedClients_.indexOf(signatureObject,(index+1))){
            auto connectedClient = connectedClients_.at(index);
            if(connectedClient.isValid()){
                list.append(connectedClient);
            }else{
                qWarning() << QString("MSessions::getConnectedClients | Critcal Error found INVALID connectedClient;THIS IS A BUG ,FIX THIS BUG IMMEDIATRY ");
            }
        }

        return list;

    }
};















/**-----------------Mgb::ConnectedUsers---------------------------------------
 *
 */
namespace Mgb {
/**
   * @brief CUsers::CUsers
   * @param parent
   */
ConnectedUsers::ConnectedUsers(QObject *parent) : QObject(parent)
  ,sessions_(new MSessions())
{
    crypto_ = Security::instance();

    ConnectionPipe::registerObject(this);

    //connect to MagabeServer
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeServer::className,RegisteredObjects::MagabeServer::Signals::serverStarted
                                ,this ,RegisteredObjects::Mgb_ConnectedUsers::Slots::onServerStarted);

    ConnectionPipe::connectSlot(RegisteredObjects::MagabeServerApp::className,RegisteredObjects::MagabeServerApp::Signals::clientLogedOut,
                                 this ,RegisteredObjects::Mgb_ConnectedUsers::Slots::onLogOut);

    //NOTE: DO NOT onServerStoped, IS ALLREADY CONNECTED

}

/**
   * @brief CUsers::~CUsers
   */
ConnectedUsers::~ConnectedUsers()
{
    ConnectionPipe::unRegisterObject(this);

    if(sessions_){
        sessions_->closeAllSessions();
        delete sessions_;
        sessions_ = nullptr;
    }
}

/**
   * @brief CUsers::authenticate
   * @param client
   * @return
   */
bool ConnectedUsers::addAuthenticatedUser(unsigned long userId, QSslSocket *client)
{

    QString mgbSessionId = getMgbSessionId(client);
    if(!mgbSessionId.isEmpty()){
        return addAuthenticatedUser(userId,mgbSessionId);
    }else{
        print("Error :Failed to add user,socket already disconnected");
        return false;
    }

}


bool ConnectedUsers::addAuthenticatedUser(unsigned long userId, const QString &mgbSessionId)
{

    const bool  authenticated = sessions_->tagAsAuthenticatedSession(mgbSessionId,userId);
    if(authenticated){

        //first
        emit newAuthentication(userId,mgbSessionId,QDateTime::currentDateTime());
        //then
        emit authenticedClientsCountChanged(authenticedClientsCount());
        emit unAuthenticatedClientsCountChanged(unAuthenticatedClient());

        //print(QString("User id = %1 :mgbSessionId = %2 autheticated").arg(userId).arg(mgbSessionId));
    }else{
        qWarning() << " bool ConnectedUsers::addAuthenticatedUser ; Authenticaton failed";
    }
    return authenticated;
}

/**
   * @brief CUsers::noOfAuthenticedUsers
   * @return
   */
int ConnectedUsers::authenticedClientsCount()
{
    return sessions_->authenticedClientsCount();
}


/**
   * @brief CUsers::noOfNonAuthenticatedUsers
   * @return
   */
int ConnectedUsers::unAuthenticatedClient()
{
    return sessions_->unAuthenticatedClientsCount();
}


/**
   * @brief CUsers::deauthenticate
   * @param client
   * @return
   */
bool ConnectedUsers::removeAuthenticatedUser(QSslSocket *client)
{
    QString mgbSessionId = getMgbSessionId(client);
    return removeAuthenticatedUser(mgbSessionId);
}


/**
   * @brief CUsers::deauthenticate
   * @param socketDescriptor
   * @return
   */
bool ConnectedUsers::removeAuthenticatedUser(const QString &mgbSessionId)
{
    bool changed = sessions_->tagAsNonAuthenticatedSession(mgbSessionId);
    if(changed){
        emit authenticedClientsCountChanged(authenticedClientsCount());
        emit unAuthenticatedClientsCountChanged(unAuthenticatedClient());
    }
    return changed;
}

/**
   * @brief CUsers::addUser
   * @param client
   * @return
   */
void ConnectedUsers::addClient(QSslSocket *client)
{
    if(!client){
        return;
    }

    if(!client->isValid()){
        return;
    }

    if(!sessions_->userExists(client)) {

        QString mgbSessionId;
        if(sessions_->addNewSession(client,mgbSessionId)){
            if(mgbSessionId.isEmpty()){
                qWarning() << "ConnectedUsers::addUser | mgbSessionId is Empty, THIS IS A BUG FIX IT";
                return;
            }

            emit newConnection(mgbSessionId);

            //connect
            connect(client,&QSslSocket::disconnected,this,&ConnectedUsers::onDisconnected);

            emit authenticedClientsCountChanged(authenticedClientsCount());
            emit unAuthenticatedClientsCountChanged(unAuthenticatedClient());

            //start authentication
            emit startAuthenticaion(mgbSessionId);
        }
    }else{;
        qWarning() << "void ConnectedUsers::addUser :: implementation error, user exists";
    }
}


bool ConnectedUsers::isAuthenticated(QString mgbSessionId)
{
    return sessions_->isAuthenticated(mgbSessionId);
}



unsigned long ConnectedUsers::getUserId(const QString &mgbSessionId)
{
    return sessions_->getUserId(mgbSessionId);
}


const QSslSocket *ConnectedUsers::getSocket(const QString &mgbSessionId)
{
    return sessions_->getSocket(mgbSessionId);
}

void ConnectedUsers::send(QString data, unsigned long userId)
{
    auto userSessions = sessions_->getUserSessions(userId);
    foreach (auto mgbSessionId, userSessions) {
        send(MagabeData(data),mgbSessionId);
    }
}


void ConnectedUsers::send(MagabeData data, const QString &mgbSessionId)
{

    if(QSslSocket * client = const_cast<QSslSocket *>(getSocket(mgbSessionId))){
        QString dataForWriting = crypto_->encryptToString(data.toByteArray()).append("\r\n");
        if(client->write(dataForWriting.toLatin1()) != -1){
#if PRINT_DATA_SENT
            print(QString("data sent[mgbSessionId =%1]=> ").arg(mgbSessionId).append(data.toByteArray()));
#endif
        }
    }
}


void ConnectedUsers::sendToAllClient(const QString &message)
{
    auto ids = sessions_->getAllMgbSessionIds();
    foreach (auto mgbSessionId, ids) {
        send(MagabeData(message),mgbSessionId);
    }
}

bool ConnectedUsers::userExists(const QString &mgbSessionId)
{
    return sessions_->userExists(mgbSessionId);
}

bool ConnectedUsers::userExists(const QSslSocket *client)
{
    return sessions_->userExists(client);
}

QString ConnectedUsers::getMgbSessionId(const QSslSocket *client)
{
    return sessions_->getMgbSessionId(client);
}

QList<QString> ConnectedUsers::getMgbSessionIds(unsigned long userId)
{
    return sessions_->getUserSessions(userId);
}

QList<const QSslSocket *> ConnectedUsers::getSockets(unsigned long userId)
{
    return sessions_->getSocketsForUserSessions(userId);
}

bool ConnectedUsers::hasConnectedClients(unsigned long userId)
{
    return sessions_->hasActiveSessions(userId);
}


void ConnectedUsers::print(const QString &text)
{
    qDebug() << text;
}


void ConnectedUsers::onDisconnected()
{
    QSslSocket *client = qobject_cast<QSslSocket *>(sender());
    if(client){
        // print("ConnectedUsers::onDisconnected(),start cleaning ....");
        QString mgbSessionId = getMgbSessionId(client);
        unsigned long userId = getUserId(mgbSessionId);//get the id

        removeUser(mgbSessionId);//do impoertant staff first
        //this starts
        emit clientDisconnected(mgbSessionId,userId,QDateTime::currentDateTime());
        //ten follow
        emit authenticedClientsCountChanged(authenticedClientsCount());
        emit unAuthenticatedClientsCountChanged(unAuthenticatedClient());

        if(!sessions_->hasActiveSessions(userId)){
            //set isActive,isActive can be used to figure out last seen, cause each time is
            //update updatedAt is also updated,so you can use updateAt as last seen
            usermodel_.updateIsActive(false,userId);
        }

    }
}

void ConnectedUsers::onServerStarted(QHostAddress address, quint16 port){
    Q_UNUSED(address)
    Q_UNUSED(port)

    emit authenticedClientsCountChanged(authenticedClientsCount());
    emit unAuthenticatedClientsCountChanged(unAuthenticatedClient());
}

void ConnectedUsers::onServerStoped()
{    qDebug() << "ConnectedUsers::onServerStoped";

    sessions_->closeAllSessions();

    emit authenticedClientsCountChanged(authenticedClientsCount());
    emit unAuthenticatedClientsCountChanged(unAuthenticatedClient());

}

bool ConnectedUsers::onAuthenticationProcessSucceded(QString mgbSessionId, unsigned long userId)
{
    return addAuthenticatedUser(userId,mgbSessionId);
}

bool ConnectedUsers::onLogOut(QString mgbSessionId)
{
    qDebug() << "ConnectedUsers::onLogOut(QString mgbSessionId)" << mgbSessionId;
    return removeAuthenticatedUser(mgbSessionId);
}



bool ConnectedUsers::removeUser(const QString &mgbSessionId)
{
    return sessions_->closeSession(mgbSessionId);
}

bool ConnectedUsers::removeUser(QSslSocket *client)
{
    return removeUser(getMgbSessionId(client));
}

}




