/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "connectedclientsmodel.h"

/** ----------------------Client-------------------------------
 * @brief Client::Client
 * @param id
 * @param mgbSessionId
 */
Client::Client(const unsigned long id, const QString &mgbSessionId)
    :isRoot_(false)
{
    id_= id;
    mgbSessionId_ = mgbSessionId;
}

Client::Client(const unsigned long id)
    :mgbSessionId_("")
    ,isRoot_(true)
{
    id_= id;
}

int Client::type() const{
    return QStandardItem::UserType + 122;
}

QVariant Client::data(int role) const{

    auto self = const_cast<Client*>(this);
    if(id_ == 0){

        if(isRoot_){
            switch (role) {
            case Qt::DisplayRole:return QVariant::fromValue(QString("Unregistered Clients"));
            default:
                return QVariant();
            }
        }else{
            ConnectedClientsModel *mdl = static_cast<ConnectedClientsModel *>(model());
            int count = 0;
            if(mdl){
                auto parent =  mdl->getRootItem(self);
                if(parent){
                    count = parent->rowCount();
                }
            }
            switch (role) {
            case Qt::DisplayRole:return QVariant::fromValue(QString("unregistered#%1").arg(count));
            case MgbSessionId : return QVariant::fromValue(mgbSessionId_);
            default:
                return QVariant();
            }
        }

    }

    const User &user = self->userModel_.getUser(id_);

    if(isRoot_){
        switch (role) {
        case Qt::DisplayRole:return QVariant::fromValue(user.username());
        default:
            return QVariant();
        }
    }else{
        switch (role) {
        case Qt::DisplayRole:return QVariant::fromValue(QString("%1").arg(mgbSessionId_));
        case IdRole : return QVariant::fromValue(user.id());
        case FirstNameRole : return QVariant::fromValue(user.firstName());
        case LastNameRole : return QVariant::fromValue(user.lastName());
        case DateOfBirthRole : return QVariant::fromValue(user.dateOfBirth());
        case GenderRole : return QVariant::fromValue(user.gender());
        case UsernameRole : return QVariant::fromValue(user.username());
        case AuthKeyRole : return QVariant::fromValue(user.authKey());
        case PasswordRole : return QVariant::fromValue(user.password());
        case EmailRole : return QVariant::fromValue(user.email());
        case PhoneNumberRole : return QVariant::fromValue(user.phoneNumber());
        case CreatedAtRole : return QVariant::fromValue(user.createdAt());
        case UpdatedAtRole : return QVariant::fromValue(user.updatedAt());
        case ProfilePicturePathRole : return QVariant::fromValue(user.profilePicturePath());
        case PositionRole : return QVariant::fromValue(user.position());
        case EducationRole : return QVariant::fromValue(user.education());
        case LocationRole : return QVariant::fromValue(user.location());
        case MgbSessionId : return QVariant::fromValue(mgbSessionId_);
        default:
            return QVariant();
        }
    }
}

unsigned long Client::id() const
{
    return id_;
}

QString Client::mgbSessionId() const
{
    return mgbSessionId_;
}

bool Client::isRoot() const
{
    return isRoot_;
}


/** ----------------ClientsModel------------------------------
 * @brief ClientsModel::ClientsModel
 * @param parent
 */
ConnectedClientsModel::ConnectedClientsModel(QObject *parent) : QStandardItemModel(parent)
{

}

void ConnectedClientsModel::swap(const QString &mgbSessionId, const unsigned long userId){
    if(mgbSessionId.isEmpty()){
        return;
    }

    remove(mgbSessionId);

    Client *item = new Client(userId,mgbSessionId);
    auto root = getRootItem(item);
    if(root){
        root->appendRow(item);
    }else{
        auto root = new Client(item->id());
        QStandardItemModel::appendRow(root);
        clients_.append(ClientHolder("",item->id(),root));
        root->appendRow(item);

    }
    clients_.append(ClientHolder(item->mgbSessionId(),item->id(),item));

}

void ConnectedClientsModel::remove(const QString &mgbSessionId){
    Client *item = getItem(mgbSessionId);
    if(item){
        QModelIndex itemIndex = indexFromItem(item);

        Client *parent = getRootItem(item);
        QModelIndex  parentIndex = QModelIndex();
        if(parent){
            parentIndex = indexFromItem(parent);

            parent->removeRow(itemIndex.row());

            if(parent->rowCount() == 0){
                ClientHolder rootHolder("",parent->id(),nullptr);
                rootHolder.setSearchBy(ClientHolder::SearchBy::UserId);

                int index = -1;

                for(index = clients_.indexOf(rootHolder);
                    index != -1;
                    index = clients_.indexOf(rootHolder,(index+1))){

                    auto client = clients_.at(index).ptr();
                    if(client){
                        if(client->isRoot()) break;
                    }

                }

                if(index != -1){
                    clients_.removeAt(index);
                }

                removeRow(parent->row());

            }
            ClientHolder clientHolder(mgbSessionId,0,nullptr);
            clientHolder.setSearchBy(ClientHolder::SearchBy::MgbSessionId);
            clients_.removeAll(clientHolder);
        }

    }
}

bool ConnectedClientsModel::hasRoot(Client *item){
    if(!item){
        return false;
    }

    ClientHolder clientHolder(item->mgbSessionId(),item->id(),item);
    clientHolder.setSearchBy(ClientHolder::SearchBy::UserId);
    return clients_.contains(clientHolder);
}

bool ConnectedClientsModel::containClientWith(const QString &mgbSessionId){
    ClientHolder clientHolder(mgbSessionId,0,nullptr);
    clientHolder.setSearchBy(ClientHolder::SearchBy::MgbSessionId);
    return clients_.contains(clientHolder);
}

Client *ConnectedClientsModel::getItem(const QString &mgbSessionId){
    ClientHolder clientHolder(mgbSessionId,0,nullptr);
    clientHolder.setSearchBy(ClientHolder::SearchBy::MgbSessionId);
    const int index = clients_.indexOf(clientHolder);
    if(index == -1){
        return nullptr;
    }else{
        return clients_.at(index).ptr();
    }
}

Client *ConnectedClientsModel::getRootItem(Client *item){
    ClientHolder clientHolder(item->mgbSessionId(),item->id(),item);
    clientHolder.setSearchBy(ClientHolder::SearchBy::UserId);
    int index = -1;

    for(index = clients_.indexOf(clientHolder);
        index != -1;
        index = clients_.indexOf(clientHolder,(index+1))){

        auto client = clients_.at(index).ptr();
        if(client){
            if(client->isRoot()) break;
        }

    }
    if(index == -1){
        return nullptr;
    }else{
        return clients_.at(index).ptr();
    }
}


/** ----------------ClientHolder-------------------------------
 * @brief ClientHolder::ClientHolder
 * @param mgbSessionId
 * @param id
 * @param ptr
 */
ClientHolder::ClientHolder(const QString &mgbSessionId, unsigned long id, Client *ptr):searchBy_(SearchBy::All){
    mgbSessionId_ = mgbSessionId;
    id_ = id;
    ptr_ = ptr;
}

bool ClientHolder::operator ==(const ClientHolder &other){
    switch (other.searchBy_) {
    case SearchBy::UserId: return (this->id_ == other.id_);
    case SearchBy::Ptr: return (this->ptr_ == other.ptr_);
    case SearchBy::MgbSessionId :return (this->mgbSessionId_ == other.mgbSessionId_);
    case SearchBy::All:
    default:
        return ((this->id_ == other.id_)
                && (this->mgbSessionId_ == other.mgbSessionId_)
                && (this->ptr_ == other.ptr_));
    }
}

bool ClientHolder::operator !=(const ClientHolder &other){
    switch (other.searchBy_) {
    case SearchBy::UserId: return (this->id_ != other.id_);
    case SearchBy::Ptr: return (this->ptr_ != other.ptr_);
    case SearchBy::MgbSessionId :return (this->mgbSessionId_ != other.mgbSessionId_);
    case SearchBy::All:
    default:
        return ((this->id_ != other.id_)
                || (this->mgbSessionId_ != other.mgbSessionId_)
                || (this->ptr_ != other.ptr_));
    }
}

unsigned long ClientHolder::id() const
{
    return id_;
}

void ClientHolder::setId(unsigned long id)
{
    id_ = id;
}

Client *ClientHolder::ptr() const
{
    return ptr_;
}

void ClientHolder::setPtr(Client *ptr)
{
    ptr_ = ptr;
}

ClientHolder::SearchBy ClientHolder::searchBy() const
{
    return searchBy_;
}

void ClientHolder::setSearchBy(const ClientHolder::SearchBy &searchBy)
{
    searchBy_ = searchBy;
}
