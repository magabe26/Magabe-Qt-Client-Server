/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef ICONS_H
#define ICONS_H

#include <QObject>
#include "QtAwesome.h"

class Icons : public QObject
{
    Q_OBJECT
    Q_ENUMS(icon)
public:
    explicit Icons(QObject *parent = nullptr) : QObject(parent),awesome(new QtAwesome(this)){
        awesome->initFontAwesome();

    }
    //options
    //QPushButton* beerButton
    //            QVariantMap options
    //            options.insert("anim", qVariantFromValue( new QtAwesomeAnimation(beerButton) ) );
    //            options.insert("color", QColor(Qt::green) );
    //            options.insert("text-off", QString(fa::squareo) );
    //            options.insert("color-off", QColor(Qt::red) );

    QIcon getIcon(QString name,QVariantMap options){
        return awesome->icon(awesome->getCodePoint(name.remove("fa::")),options);
    }

    Q_PROPERTY(QString  fa_500px READ fa_500px NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  addressbook READ addressbook NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  addressbooko READ addressbooko NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  addresscard READ addresscard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  addresscardo READ addresscardo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  adjust READ adjust NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  adn READ adn NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  aligncenter READ aligncenter NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  alignjustify READ alignjustify NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  alignleft READ alignleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  alignright READ alignright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  amazon READ amazon NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ambulance READ ambulance NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  americansignlanguageinterpreting READ americansignlanguageinterpreting NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  anchor READ anchor NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  android READ android NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angellist READ angellist NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angledoubledown READ angledoubledown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angledoubleleft READ angledoubleleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angledoubleright READ angledoubleright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angledoubleup READ angledoubleup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angledown READ angledown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angleleft READ angleleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angleright READ angleright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  angleup READ angleup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  apple READ apple NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  archive READ archive NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  areachart READ areachart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircledown READ arrowcircledown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircleleft READ arrowcircleleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircleodown READ arrowcircleodown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircleoleft READ arrowcircleoleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircleoright READ arrowcircleoright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircleoup READ arrowcircleoup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircleright READ arrowcircleright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowcircleup READ arrowcircleup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowdown READ arrowdown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowleft READ arrowleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowright READ arrowright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowup READ arrowup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrows READ arrows NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowsalt READ arrowsalt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowsh READ arrowsh NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  arrowsv READ arrowsv NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  aslinterpreting READ aslinterpreting NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  assistivelisteningsystems READ assistivelisteningsystems NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  asterisk READ asterisk NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  at READ at NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  audiodescription READ audiodescription NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  automobile READ automobile NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  backward READ backward NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  balancescale READ balancescale NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ban READ ban NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bandcamp READ bandcamp NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bank READ bank NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  barchart READ barchart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  barcharto READ barcharto NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  barcode READ barcode NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bars READ bars NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bath READ bath NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bathtub READ bathtub NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  battery READ battery NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  battery0 READ battery0 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  battery1 READ battery1 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  battery2 READ battery2 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  battery3 READ battery3 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  battery4 READ battery4 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  batteryempty READ batteryempty NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  batteryfull READ batteryfull NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  batteryhalf READ batteryhalf NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  batteryQuarter READ batteryQuarter NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  batterythreeQuarters  READ batterythreeQuarters NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bed READ bed NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  beer READ beer NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  behance READ behance NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  behancesQuare READ behancesQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bell READ bell NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bello READ bello NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bellslash READ bellslash NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bellslasho READ bellslasho NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bicycle READ bicycle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  binoculars READ binoculars NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  birthdaycake READ birthdaycake NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bitbucket READ bitbucket NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bitbucketsQuare READ bitbucketsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bitcoin READ bitcoin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  blacktie READ blacktie NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  blind READ blind NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bluetooth READ bluetooth NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bluetoothb READ bluetoothb NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bold READ bold NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bolt READ bolt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bomb READ bomb NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  book READ book NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bookmark READ bookmark NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bookmarko READ bookmarko NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  braille READ braille NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  briefcase READ briefcase NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  btc READ btc NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bug READ bug NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  building READ building NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  buildingo READ buildingo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bullhorn READ bullhorn NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bullseye READ bullseye NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  bus READ bus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  buysellads READ buysellads NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cab READ cab NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  calculator READ calculator NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  calendar READ calendar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  calendarcheck READ calendarcheck NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  calendarminus READ calendarminus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  calendaro READ calendaro NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  calendarplus READ calendarplus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  calendartimes READ calendartimes NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  camera READ camera NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cameraretro READ cameraretro NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  car READ car NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretdown READ caretdown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretleft READ caretleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretright READ caretright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretsQuareodown READ caretsQuareodown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretsQuareolef READ caretsQuareoleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretsQuareoleft READ caretsQuareoright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretsQuareoup READ caretsQuareoup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  caretup READ caretup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cartarrowdown READ cartarrowdown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cartplus READ cartplus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cc READ cc NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccamex READ ccamex NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccdinersclub READ ccdinersclub NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccdiscover READ ccdiscover NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccjcb READ ccjcb NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccmastercard READ ccmastercard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccpaypal READ ccpaypal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccstripe READ ccstripe NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ccvisa READ ccvisa NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  certificate READ certificate NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chain READ chain NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chainbroken READ chainbroken NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  check READ check NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  checkcircle READ checkcircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  checkcircleo READ checkcircleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  checksQuare READ checksQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  checksQuareo READ checksQuareo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevroncircledown READ chevroncircledown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevroncircleleft READ chevroncircleleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevroncircleright READ chevroncircleright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevroncircleup READ chevroncircleup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevrondown READ chevrondown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevronleft READ chevronleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevronright READ chevronright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chevronup READ chevronup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  child READ child NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  chrome READ chrome NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  circle READ circle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  circleo READ circleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  circleonotch READ circleonotch NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  circlethin READ circlethin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  clipboard READ clipboard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  clocko READ clocko NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  clone READ clone NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  close READ close NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cloud READ cloud NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  clouddownload READ clouddownload NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cloudupload READ cloudupload NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cny READ cny NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  code READ code NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  codefork READ codefork NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  codepen READ codepen NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  codiepie READ codiepie NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  coffee READ coffee NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cog READ cog NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cogs READ cogs NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  columns READ columns NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  comment READ comment NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  commento READ commento NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  commenting READ commenting NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  commentingo READ commentingo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  comments READ comments NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  commentso READ commentso NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  compass READ compass NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  compress READ compress NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  connectdevelop READ connectdevelop NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  contao READ contao NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  copy READ copy NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  copyright READ copyright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  creativecommons READ creativecommon NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  creditcard READ creditcard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  creditcardalt READ creditcardal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  crop READ crop NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  crosshairs READ crosshairs NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  css3 READ css3 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cube READ cube NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cubes READ cubes NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cut READ cut NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  cutlery READ cutlery NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  dashboard READ dashboard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  dashcube READ dashcube NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  database READ database NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  deaf READ deaf NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  deafness READ deafness NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  dedent READ dedent NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  delicious READ delicious NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  desktop READ desktop NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  deviantart READ deviantart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  diamond READ diamond NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  digg READ digg NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  dollar READ dollar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  dotcircleo READ dotcircleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  download READ download NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  dribbble READ dribbble NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  driverslicense READ driverslicens NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  driverslicenseo READ driverslicense NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  dropbox READ dropbox NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  drupal READ drupal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  edge READ edge NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  edit READ edit NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  eercast READ eercast NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  eject READ eject NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ellipsish READ ellipsish NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ellipsisv READ ellipsisv NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  empire READ empire NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  envelope READ envelope NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  envelopeo READ envelopeo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  envelopeopen READ envelopeopen NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  envelopeopeno READ envelopeopeno NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  envelopesQuare READ envelopesQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  envira READ envira NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  eraser READ eraser NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  etsy READ etsy NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  eur READ eur NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  euro READ euro NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  exchange READ exchange NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  exclamation READ exclamation NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  exclamationcircle READ exclamationcircl NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  exclamationtriangle  READ exclamationtriangle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  expand READ expand NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  expeditedssl READ expeditedssl NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  externallink READ externallink NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  externallinksQuare READ externallinksQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  eye READ eye NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  eyeslash READ eyeslash NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  eyedropper READ eyedropper NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fa READ fa NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  facebook READ facebook NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  facebookf READ facebookf NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  facebookofficial READ facebookofficia NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  facebooksQuare READ facebooksQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fastbackward READ fastbackward NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fastforward READ fastforward NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fax READ fax NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  feed READ feed NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  female READ female NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fighterjet READ fighterjet NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  file READ file NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filearchiveo READ filearchiveo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fileaudioo READ fileaudioo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filecodeo READ filecodeo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fileexcelo READ fileexcelo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fileimageo READ fileimageo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filemovieo READ filemovieo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fileo READ fileo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filepdfo READ filepdfo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filephotoo READ filephotoo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filepictureo READ filepictureo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filepowerpoint READ filepowerpoint NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filesoundo READ filesoundo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filetext READ filetext NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filetexto READ filetexto NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filevideoo READ filevideoo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filewordo READ filewordo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filezipo READ filezipo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fileso READ fileso NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  film READ film NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  filter READ filter NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fire READ fire NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fireextinguishe READ fireextinguishe NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  firefox READ firefox NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  firstorder READ firstorder NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  flag READ flag NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  flagcheckered READ flagcheckere NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  flago READ flago NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  flash READ flash NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  flask READ flask NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  flickr READ flickr NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  floppyo READ floppyo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  folder READ folder NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  foldero READ foldero NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  folderopen READ folderopen NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  folderopeno READ folderopeno NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  font READ font NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fontawesome READ fontawesome NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fonticons READ fonticons NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fortawesome READ fortawesome NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  forumbee READ forumbee NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  forward READ forward NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  foursQuare READ foursQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  freecodecamp READ freecodecamp NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  frowno READ frowno NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  futbolo READ futbolo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gamepad READ gamepad NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gavel READ gavel NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gbp READ gbp NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ge READ ge NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gear READ gear NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gears READ gears NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  genderless READ genderless NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  getpocket READ getpocket NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gg READ gg NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ggcircle READ ggcircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gift READ gift NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  git READ git NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gitsQuare READ gitsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  github READ github NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  githubalt READ githubalt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  githubsQuare READ githubsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gitlab READ gitlab NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gittip READ gittip NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  glass READ glass NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  glide READ glide NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  glideg READ glideg NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  globe READ globe NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  google READ google NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  googleplus READ googleplus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  googlepluscircle READ googlepluscircl NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  googleplusofficial READ googleplusofficia NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  googleplussQuare READ googleplussQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  googlewallet READ googlewallet NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  graduationcap READ graduationca NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  gratipay READ gratipay NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  grav READ grav NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  group READ group NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hsQuare READ hsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hackernews READ hackernews NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handgrabo READ handgrabo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handlizardo READ handlizardo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handodown READ handodown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handoleft READ handoleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handoright READ handoright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handoup READ handoup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handpapero READ handpapero NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handpeaceo READ handpeaceo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handpointero READ handpointero NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handrocko READ handrocko NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handscissorso READ handscissors NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handspocko READ handspocko NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handstopo READ handstopo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  handshakeo READ handshakeo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hardofhearing READ hardofhearin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hashtag READ hashtag NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hddo READ hddo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  header READ header NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  headphones READ headphones NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  heart READ heart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hearto READ hearto NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  heartbeat READ heartbeat NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  history READ history NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  home READ home NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hospitalo READ hospitalo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hotel READ hotel NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglass READ hourglass NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglass1 READ hourglass1 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglass2 READ hourglass2 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglass3 READ hourglass3 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglassend READ hourglassend NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglasshalf READ hourglasshal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglasso READ hourglasso NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  hourglassstary READ hourglassstar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  houzz READ houzz NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  html5 READ html5 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  icursor READ icursor NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  idbadge READ idbadge NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  idcard READ idcard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  idcardo READ idcardo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ils READ ils NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  image READ image NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  imdb READ imdb NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  inbox READ inbox NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  indent READ indent NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  industry READ industry NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  info READ info NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  infocircle READ infocircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  inr READ inr NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  instagram READ instagram NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  institution READ institution NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  internetexplorer READ internetexplore NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  intersex READ intersex NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ioxhost READ ioxhost NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  italic READ italic NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  joomla READ joomla NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  jpy READ jpy NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  jsfiddle READ jsfiddle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  key READ key NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  keyboardo READ keyboardo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  krw READ krw NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  language READ language NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  laptop READ laptop NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lastfm READ lastfm NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lastfmsQuare READ lastfmsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  leaf READ leaf NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  leanpub READ leanpub NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  legal READ legal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lemono READ lemono NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  leveldown READ leveldown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  levelup READ levelup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lifebouy READ lifebouy NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lifebuoy READ lifebuoy NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lifering READ lifering NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lifesaver READ lifesaver NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lightbulbo READ lightbulbo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  linechart READ linechart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  link READ link NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  linkedin READ linkedin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  linkedinsQuare READ linkedinsQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  linode READ linode NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fa_linux READ fa_linux NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  list READ list NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  listalt READ listalt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  listol READ listol NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  listul READ listul NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  locationarrow READ locationarro NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lock READ lock NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  longarrowdown READ longarrowdow NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  longarrowleft READ longarrowlef NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  longarrowright READ longarrowrigh NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  longarrowup READ longarrowup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  lowvision READ lowvision NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  magic READ magic NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  magnet READ magnet NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mailforward READ mailforward NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mailreply READ mailreply NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mailreplyall READ mailreplyall NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  male READ male NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  map READ map NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mapmarker READ mapmarker NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mapo READ mapo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mappin READ mappin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mapsigns READ mapsigns NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mars READ mars NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  marsdouble READ marsdouble NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  marsstroke READ marsstroke NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  marsstrokeh READ marsstrokeh NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  marsstrokev READ marsstrokev NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  maxcdn READ maxcdn NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  meanpath READ meanpath NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  medium READ medium NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  medkit READ medkit NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  meetup READ meetup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  meho READ meho NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mercury READ mercury NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  microchip READ microchip NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  microphone READ microphone NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   microphoneslash READ microphoneslas NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  minus READ minus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  minuscircle READ minuscircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  minussQuare READ minussQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  minussQuareo READ minussQuareo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mixcloud READ mixcloud NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mobile READ mobile NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mobilephone READ mobilephone NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  modx READ modx NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  money READ money NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  moono READ moono NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mortarboard READ mortarboard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  motorcycle READ motorcycle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  mousepointer READ mousepointer NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  music READ music NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  navicon READ navicon NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  neuter READ neuter NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  newspapero READ newspapero NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  objectgroup READ objectgroup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   objectungroup READ objectungrou NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  odnoklassniki READ odnoklassnik NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  odnoklassnikisQuare  READ odnoklassnikisQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  opencart READ opencart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  openid READ openid NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  opera READ opera NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  optinmonster READ optinmonster NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  outdent READ outdent NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pagelines READ pagelines NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paintbrush READ paintbrush NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paperplane READ paperplane NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paperplaneo READ paperplaneo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paperclip READ paperclip NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paragraph READ paragraph NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paste READ paste NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pause READ pause NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pausecircle READ pausecircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pausecircleo READ pausecircleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paw READ paw NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  paypal READ paypal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pencil READ pencil NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pencilsQuare READ pencilsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pencilsQuareo READ pencilsQuare0 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  percent READ percent NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  phone READ phone NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  phonesQuare READ phonesQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  photo READ photo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pictureo READ pictureo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  piechart READ piechart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  piedpiper READ piedpiper NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  piedpiperalt READ piedpiperalt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  piedpiperpp READ piedpiperpp NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pinterest READ pinterest NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pinterestp READ pinterestp NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   pinterestsQuare READ pinterestsQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  plane READ plane NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  play READ play NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  playcircle READ playcircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  playcircleo READ playcircleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  plug READ plug NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  plus READ plus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  pluscircle READ pluscircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  plussQuare READ plussQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  plussQuareo READ plussQuareo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  podcast READ podcast NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  poweroff READ poweroff NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  print READ print NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  producthunt READ producthunt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  puzzlepiece READ puzzlepiece NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  QQ READ QQ NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  Qrcode READ Qrcode NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  Question READ Question NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  Questioncircl READ Questioncircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  Questioncircle READ Questioncircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  Quora READ Quora NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  Quoteleft READ Quoteleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  Quoteright READ Quoteright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ra READ ra NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  random READ random NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ravelry READ ravelry NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rebel READ rebel NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  recycle READ recycle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  reddit READ reddit NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  redditalien READ redditalien NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  redditsQuare READ redditsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  refresh READ refresh NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  registered READ registered NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  remove READ remove NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  renren READ renren NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  reorder READ reorder NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  repeat READ repeat NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  reply READ reply NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  replyall READ replyall NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  resistance READ resistance NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  retweet READ retweet NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rmb READ rmb NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  road READ road NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rocket READ rocket NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rotateleft READ rotateleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rotateright READ rotateright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rouble READ rouble NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rss READ rss NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rsssQuare READ rsssQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rub READ rub NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ruble READ ruble NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  rupee READ rupee NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  s15 READ s15 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  safari READ safari NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  save READ save NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  scissors READ scissors NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  scribd READ scribd NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  search READ search NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  searchminus READ searchminus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  searchplus READ searchplus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sellsy READ sellsy NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  send READ send NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sendo READ sendo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  server READ server NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  share READ share NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sharealt READ sharealt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   sharealtsQuare READ sharealtsQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sharesQuare READ sharesQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sharesQuareo READ sharesQuareo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  shekel READ shekel NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sheQel READ sheQel NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  shield READ shield NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ship READ ship NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  shirtsinbulk READ shirtsinbulk NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  shoppingbag READ shoppingbag NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  shoppingbasket READ shoppingbaske NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  shoppingcart READ shoppingcart NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  shower READ shower NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  signin READ signin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  signlanguage READ signlanguage NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  signout READ signout NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  signal READ signal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  signing READ signing NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  simplybuilt READ simplybuilt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sitemap READ sitemap NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  skyatlas READ skyatlas NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  skype READ skype NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  slack READ slack NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sliders READ sliders NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  slideshare READ slideshare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  smileo READ smileo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  snapchat READ snapchat NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   snapchatghost READ snapchatghos NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   snapchatsQuare READ snapchatsQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  snowflakeo READ snowflakeo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  soccerballo READ soccerballo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sort READ sort NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   sortalphaasc READ sortalphaasc NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sortalphadesc READ sortalphades  NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   sortamountasc READ sortamountas NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   sortamountdesc READ sortamountdes NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sortasc READ sortasc NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sortdesc READ sortdesc NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sortdown READ sortdown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sortnumericasc READ sortnumericas NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sortnumericdesc READ sortnumericdes NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sortup READ sortup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  soundcloud READ soundcloud NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  spaceshuttle READ spaceshuttle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  spinner READ spinner NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  spoon READ spoon NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  spotify READ spotify NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sQuare READ sQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  sQuareo READ sQuareo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString   stackexchange READ stackexchang NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stackoverflow READ stackoverflo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  star READ star NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  starhalf READ starhalf NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  starhalfempty READ starhalfempt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  starhalffull READ starhalffull NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  starhalfo READ starhalfo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  staro READ staro NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  steam READ steam NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  steamsQuare READ steamsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stepbackward READ stepbackward NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stepforward READ stepforward NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stethoscope READ stethoscope NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stickynote READ stickynote NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stickynoteo READ stickynoteo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stop READ stop NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stopcircle READ stopcircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stopcircleo READ stopcircleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  streetview READ streetview NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  strikethrough READ strikethroug NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stumbleupon READ stumbleupon NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  stumbleuponcircle READ stumbleuponcircl NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  subscript READ subscript NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  subway READ subway NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  suitcase READ suitcase NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  suno READ suno NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  superpowers READ superpowers NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  superscript READ superscript NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  support READ support NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  table READ table NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tablet READ tablet NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tachometer READ tachometer NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tag READ tag NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tags READ tags NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tasks READ tasks NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  taxi READ taxi NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  telegram READ telegram NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  television READ television NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tencentweibo READ tencentweibo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  terminal READ terminal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  textheight READ textheight NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  textwidth READ textwidth NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  th READ th NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thlarge READ thlarge NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thlist READ thlist NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  themeisle READ themeisle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometer READ thermometer NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometer0 READ thermometer0 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometer1 READ thermometer1 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometer2 READ thermometer2 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometer3 READ thermometer3 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometer4 READ thermometer4 NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometerempty READ thermometerempt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometerfull READ thermometerful NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometerhalf READ thermometerhal  NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometerQuarte READ thermometerQuarte NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thermometerthreeQuarters READ thermometerthreeQuarters NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thumbtack READ thumbtack NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thumbsdown READ thumbsdown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thumbsodown READ thumbsodown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thumbsoup READ thumbsoup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  thumbsup READ thumbsup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ticket READ ticket NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  times READ times NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  timescircle READ timescircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  timescircleo READ timescircleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  timesrectangl READ timesrectangle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  timesrectangle READ timesrectangle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tint READ tint NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  toggledown READ toggledown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  toggleleft READ toggleleft NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  toggleoff READ toggleoff NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  toggleon READ toggleon NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  toggleright READ toggleright NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  toggleup READ toggleup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  trademark READ trademark NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  train READ train NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  transgender READ transgender NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  transgenderal READ transgenderal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  trash READ trash NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  trasho READ trasho NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tree READ tree NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  trello READ trello NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tripadvisor READ tripadvisor NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  trophy READ trophy NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  truck READ truck NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  fa_try READ fa_try NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tty READ tty NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tumblr READ tumblr NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tumblrsQuare READ tumblrsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  turkishlira READ turkishlira NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  tv READ tv NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  twitch READ twitch NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  twitter READ twitter NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  twittersQuar READ twittersQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  umbrella READ umbrella NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  underline READ underline NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  undo READ undo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  universalacces READ universalacces NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  university READ university NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  unlink READ unlink NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  unlock READ unlock NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  unlockalt READ unlockalt NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  unsorted READ unsorted NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  upload READ upload NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usb READ usb NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usd READ usd NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  user READ user NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usercircle READ usercircle NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usercircleo READ usercircleo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usermd READ usermd NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usero READ usero NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  userplus READ userplus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usersecret READ usersecret NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  usertimes READ usertimes NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  users READ users NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  vcard READ vcard NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  vcardo READ vcardo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  venus READ venus NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  venusdouble READ venusdouble NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  venusmars READ venusmars NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  viacoin READ viacoin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  viadeo READ viadeo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  viadeosQuare READ viadeosQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  videocamera READ videocamera NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  vimeo READ vimeo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  vimeosQuare READ vimeosQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  vine READ vine NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  vk READ vk NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  volumecontrolphone READ volumecontrolphon NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  volumedown READ volumedown NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  volumeoff READ volumeoff NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  volumeup READ volumeup NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  warning READ warning NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wechat READ wechat NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  weibo READ weibo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  weixin READ weixin NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  whatsapp READ whatsapp NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wheelchair READ wheelchair NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wheelchairal READ wheelchairal NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wifi READ wifi NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wikipediaw READ wikipediaw NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  windowclose READ windowclose NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  windowcloseo READ windowcloseo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  windowmaximize READ windowmaximiz NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  windowminimize READ windowminimiz NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  windowrestor READ windowrestor NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  windows READ windows NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  won READ won NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wordpress READ wordpress NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wpbeginner READ wpbeginner NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wpexplorer READ wpexplorer NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wpforms READ wpforms NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  wrench READ wrench NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  xing READ xing NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  xingsQuare READ xingsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ycombinator READ ycombinator NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ycombinatorsQuar READ ycombinatorsQuar NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  yahoo READ yahoo NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  yc READ yc NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  ycsQuare READ ycsQuare NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  yelp READ yelp NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  yen READ yen NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  yoast READ yoast NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  youtube READ youtube NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  youtubeplay READ youtubeplay NOTIFY canNotChangeIcon)
    Q_PROPERTY(QString  youtubesQuar READ youtubesQuar NOTIFY canNotChangeIcon)

    QString fa_500px() { return QString("fa::fa_500px");}
    QString addressbook() { return QString("fa::addressbook");}
    QString addressbooko() { return QString("fa::addressbooko");}
    QString addresscard() { return QString("fa::addresscard");}
    QString addresscardo() { return QString("fa::addresscardo");}
    QString adjust() { return QString("fa::adjust");}
    QString adn() { return QString("fa::adn");}
    QString aligncenter() { return QString("fa::aligncenter");}
    QString alignjustify() { return QString("fa::alignjustify");}
    QString alignleft() { return QString("fa::alignleft");}
    QString alignright() { return QString("fa::alignright");}
    QString amazon() { return QString("fa::amazon");}
    QString ambulance() { return QString("fa::ambulance");}
    QString americansignlanguageinterpreting() { return QString("fa::americansignlanguageinterpreting");}
    QString anchor() { return QString("fa::anchor");}
    QString android() { return QString("fa::android");}
    QString angellist() { return QString("fa::angellist");}
    QString angledoubledown() { return QString("fa::angledoubledown");}
    QString angledoubleleft() { return QString("fa::angledoubleleft");}
    QString angledoubleright() { return QString("fa::angledoubleright");}
    QString angledoubleup() { return QString("fa::angledoubleup");}
    QString angledown() { return QString("fa::angledown");}
    QString angleleft() { return QString("fa::angleleft");}
    QString angleright() { return QString("fa::angleright");}
    QString angleup() { return QString("fa::angleup");}
    QString apple() { return QString("fa::apple");}
    QString archive() { return QString("fa::archive");}
    QString areachart() { return QString("fa::areachart");}
    QString arrowcircledown() { return QString("fa::arrowcircledown");}
    QString arrowcircleleft() { return QString("fa::arrowcircleleft");}
    QString arrowcircleodown() { return QString("fa::arrowcircleodown");}
    QString arrowcircleoleft() { return QString("fa::arrowcircleoleft");}
    QString arrowcircleoright() { return QString("fa::arrowcircleoright");}
    QString arrowcircleoup() { return QString("fa::arrowcircleoup");}
    QString arrowcircleright() { return QString("fa::arrowcircleright");}
    QString arrowcircleup() { return QString("fa::arrowcircleup");}
    QString arrowdown() { return QString("fa::arrowdown");}
    QString arrowleft() { return QString("fa::arrowleft");}
    QString arrowright() { return QString("fa::arrowright");}
    QString arrowup() { return QString("fa::arrowup");}
    QString arrows() { return QString("fa::arrows");}
    QString arrowsalt() { return QString("fa::arrowsalt");}
    QString arrowsh() { return QString("fa::arrowsh");}
    QString arrowsv() { return QString("fa::arrowsv");}
    QString aslinterpreting() { return QString("fa::aslinterpreting");}
    QString assistivelisteningsystems() { return QString("fa::assistivelisteningsystems");}
    QString asterisk() { return QString("fa::asterisk");}
    QString at() { return QString("fa::at");}
    QString audiodescription() { return QString("fa::audiodescription");}
    QString automobile() { return QString("fa::automobile");}
    QString backward() { return QString("fa::backward");}
    QString balancescale() { return QString("fa::balancescale");}
    QString ban() { return QString("fa::ban");}
    QString bandcamp() { return QString("fa::bandcamp");}
    QString bank() { return QString("fa::bank");}
    QString barchart() { return QString("fa::barchart");}
    QString barcharto() { return QString("fa::barcharto");}
    QString barcode() { return QString("fa::barcode");}
    QString bars() { return QString("fa::bars");}
    QString bath() { return QString("fa::bath");}
    QString bathtub() { return QString("fa::bathtub");}
    QString battery() { return QString("fa::battery");}
    QString battery0() { return QString("fa::battery0");}
    QString battery1() { return QString("fa::battery1");}
    QString battery2() { return QString("fa::battery2");}
    QString battery3() { return QString("fa::battery3");}
    QString battery4() { return QString("fa::battery4");}
    QString batteryempty() { return QString("fa::batteryempty");}
    QString batteryfull() { return QString("fa::batteryfull");}
    QString batteryhalf() { return QString("fa::batteryhalf");}
    QString batteryQuarter() { return QString("fa::batteryQuarter");}
    QString batterythreeQuarters(){ return QString("fa::batterythreeQuarters");}
    QString bed() { return QString("fa::bed");}
    QString beer() { return QString("fa::beer");}
    QString behance() { return QString("fa::behance");}
    QString behancesQuare() { return QString("fa::behancesQuare");}
    QString bell() { return QString("fa::bell");}
    QString bello() { return QString("fa::bello");}
    QString bellslash() { return QString("fa::bellslash");}
    QString bellslasho() { return QString("fa::bellslasho");}
    QString bicycle() { return QString("fa::bicycle");}
    QString binoculars() { return QString("fa::binoculars");}
    QString birthdaycake() { return QString("fa::birthdaycake");}
    QString bitbucket() { return QString("fa::bitbucket");}
    QString bitbucketsQuare() { return QString("fa::bitbucketsQuare");}
    QString bitcoin() { return QString("fa::bitcoin");}
    QString blacktie() { return QString("fa::blacktie");}
    QString blind() { return QString("fa::blind");}
    QString bluetooth() { return QString("fa::bluetooth");}
    QString bluetoothb() { return QString("fa::bluetoothb");}
    QString bold() { return QString("fa::bold");}
    QString bolt() { return QString("fa::bolt");}
    QString bomb() { return QString("fa::bomb");}
    QString book() { return QString("fa::book");}
    QString bookmark() { return QString("fa::bookmark");}
    QString bookmarko() { return QString("fa::bookmarko");}
    QString braille() { return QString("fa::braille");}
    QString briefcase() { return QString("fa::briefcase");}
    QString btc() { return QString("fa::btc");}
    QString bug() { return QString("fa::bug");}
    QString building() { return QString("fa::building");}
    QString buildingo() { return QString("fa::buildingo");}
    QString bullhorn() { return QString("fa::bullhorn");}
    QString bullseye() { return QString("fa::bullseye");}
    QString bus() { return QString("fa::bus");}
    QString buysellads() { return QString("fa::buysellads");}
    QString cab() { return QString("fa::cab");}
    QString calculator() { return QString("fa::calculator");}
    QString calendar() { return QString("fa::calendar");}
    QString calendarcheck() { return QString("fa::calendarchecko");}
    QString calendarminus() { return QString("fa::calendarminuso");}
    QString calendaro() { return QString("fa::calendaro");}
    QString calendarplus() { return QString("fa::calendarpluso");}
    QString calendartimes() { return QString("fa::calendartimeso");}
    QString camera() { return QString("fa::camera");}
    QString cameraretro() { return QString("fa::cameraretro");}
    QString car() { return QString("fa::car");}
    QString caretdown() { return QString("fa::caretdown");}
    QString caretleft() { return QString("fa::caretleft");}
    QString caretright() { return QString("fa::caretright");}
    QString caretsQuareodown() { return QString("fa::caretsQuareodown");}
    QString caretsQuareoleft() { return QString("fa::caretsQuareoleft");}
    QString caretsQuareoright() { return QString("fa::caretsQuareoright");}
    QString caretsQuareoup() { return QString("fa::caretsQuareoup");}
    QString caretup() { return QString("fa::caretup");}
    QString cartarrowdown() { return QString("fa::cartarrowdown");}
    QString cartplus() { return QString("fa::cartplus");}
    QString cc() { return QString("fa::cc");}
    QString ccamex() { return QString("fa::ccamex");}
    QString ccdinersclub() { return QString("fa::ccdinersclub");}
    QString ccdiscover() { return QString("fa::ccdiscover");}
    QString ccjcb() { return QString("fa::ccjcb");}
    QString ccmastercard() { return QString("fa::ccmastercard");}
    QString ccpaypal() { return QString("fa::ccpaypal");}
    QString ccstripe() { return QString("fa::ccstripe");}
    QString ccvisa() { return QString("fa::ccvisa");}
    QString certificate() { return QString("fa::certificate");}
    QString chain() { return QString("fa::chain");}
    QString chainbroken() { return QString("fa::chainbroken");}
    QString check() { return QString("fa::check");}
    QString checkcircle() { return QString("fa::checkcircle");}
    QString checkcircleo() { return QString("fa::checkcircleo");}
    QString checksQuare() { return QString("fa::checksQuare");}
    QString checksQuareo() { return QString("fa::checksQuareo");}
    QString chevroncircledown() { return QString("fa::chevroncircledown");}
    QString chevroncircleleft() { return QString("fa::chevroncircleleft");}
    QString chevroncircleright() { return QString("fa::chevroncircleright");}
    QString chevroncircleup() { return QString("fa::chevroncircleup");}
    QString chevrondown() { return QString("fa::chevrondown");}
    QString chevronleft() { return QString("fa::chevronleft");}
    QString chevronright() { return QString("fa::chevronright");}
    QString chevronup() { return QString("fa::chevronup");}
    QString child() { return QString("fa::child");}
    QString chrome() { return QString("fa::chrome");}
    QString circle() { return QString("fa::circle");}
    QString circleo() { return QString("fa::circleo");}
    QString circleonotch() { return QString("fa::circleonotch");}
    QString circlethin() { return QString("fa::circlethin");}
    QString clipboard() { return QString("fa::clipboard");}
    QString clocko() { return QString("fa::clocko");}
    QString clone() { return QString("fa::clone");}
    QString close() { return QString("fa::close");}
    QString cloud() { return QString("fa::cloud");}
    QString clouddownload() { return QString("fa::clouddownload");}
    QString cloudupload() { return QString("fa::cloudupload");}
    QString cny() { return QString("fa::cny");}
    QString code() { return QString("fa::code");}
    QString codefork() { return QString("fa::codefork");}
    QString codepen() { return QString("fa::codepen");}
    QString codiepie() { return QString("fa::codiepie");}
    QString coffee() { return QString("fa::coffee");}
    QString cog() { return QString("fa::cog");}
    QString cogs() { return QString("fa::cogs");}
    QString columns() { return QString("fa::columns");}
    QString comment() { return QString("fa::comment");}
    QString commento() { return QString("fa::commento");}
    QString commenting() { return QString("fa::commenting");}
    QString commentingo() { return QString("fa::commentingo");}
    QString comments() { return QString("fa::comments");}
    QString commentso() { return QString("fa::commentso");}
    QString compass() { return QString("fa::compass");}
    QString compress() { return QString("fa::compress");}
    QString connectdevelop() { return QString("fa::connectdevelop");}
    QString contao() { return QString("fa::contao");}
    QString copy() { return QString("fa::copy");}
    QString copyright() { return QString("fa::copyright");}
    QString creativecommon() { return QString("fa::creativecommons");}
    QString creditcard() { return QString("fa::creditcard");}
    QString creditcardal() { return QString("fa::creditcardalt");}
    QString crop() { return QString("fa::crop");}
    QString crosshairs() { return QString("fa::crosshairs");}
    QString css3() { return QString("fa::css3");}
    QString cube() { return QString("fa::cube");}
    QString cubes() { return QString("fa::cubes");}
    QString cut() { return QString("fa::cut");}
    QString cutlery() { return QString("fa::cutlery");}
    QString dashboard() { return QString("fa::dashboard");}
    QString dashcube() { return QString("fa::dashcube");}
    QString database() { return QString("fa::database");}
    QString deaf() { return QString("fa::deaf");}
    QString deafness() { return QString("fa::deafness");}
    QString dedent() { return QString("fa::dedent");}
    QString delicious() { return QString("fa::delicious");}
    QString desktop() { return QString("fa::desktop");}
    QString deviantart() { return QString("fa::deviantart");}
    QString diamond() { return QString("fa::diamond");}
    QString digg() { return QString("fa::digg");}
    QString dollar() { return QString("fa::dollar");}
    QString dotcircleo() { return QString("fa::dotcircleo");}
    QString download() { return QString("fa::download");}
    QString dribbble() { return QString("fa::dribbble");}
    QString driverslicens() { return QString("fa::driverslicense");}
    QString driverslicense() { return QString("fa::driverslicenseo");}
    QString dropbox() { return QString("fa::dropbox");}
    QString drupal() { return QString("fa::drupal");}
    QString edge() { return QString("fa::edge");}
    QString edit() { return QString("fa::edit");}
    QString eercast() { return QString("fa::eercast");}
    QString eject() { return QString("fa::eject");}
    QString ellipsish() { return QString("fa::ellipsish");}
    QString ellipsisv() { return QString("fa::ellipsisv");}
    QString empire() { return QString("fa::empire");}
    QString envelope() { return QString("fa::envelope");}
    QString envelopeo() { return QString("fa::envelopeo");}
    QString envelopeopen() { return QString("fa::envelopeopen");}
    QString envelopeopeno() { return QString("fa::envelopeopeno");}
    QString envelopesQuar() { return QString("fa::envelopesQuare");}
    QString envira() { return QString("fa::envira");}
    QString eraser() { return QString("fa::eraser");}
    QString etsy() { return QString("fa::etsy");}
    QString eur() { return QString("fa::eur");}
    QString euro() { return QString("fa::euro");}
    QString exchange() { return QString("fa::exchange");}
    QString exclamation() { return QString("fa::exclamation");}
    QString exclamationcircl() { return QString("fa::exclamationcircle");}
    QString exclamationtriangle() {return QString("fa::exclamationtriangle");}
    QString expand() { return QString("fa::expand");}
    QString expeditedssl() { return QString("fa::expeditedssl");}
    QString externallink() { return QString("fa::externallink");}
    QString externallinksQuar() { return QString("fa::externallinksQuare");}
    QString eye() { return QString("fa::eye");}
    QString eyeslash() { return QString("fa::eyeslash");}
    QString eyedropper() { return QString("fa::eyedropper");}
    QString fa() { return QString("fa::fa");}
    QString facebook() { return QString("fa::facebook");}
    QString facebookf() { return QString("fa::facebookf");}
    QString facebookofficia() { return QString("fa::facebookofficial");}
    QString facebooksQuar() { return QString("fa::facebooksQuare");}
    QString fastbackward() { return QString("fa::fastbackward");}
    QString fastforward() { return QString("fa::fastforward");}
    QString fax() { return QString("fa::fax");}
    QString feed() { return QString("fa::feed");}
    QString female() { return QString("fa::female");}
    QString fighterjet() { return QString("fa::fighterjet");}
    QString file() { return QString("fa::file");}
    QString filearchiveo() { return QString("fa::filearchiveo");}
    QString fileaudioo() { return QString("fa::fileaudioo");}
    QString filecodeo() { return QString("fa::filecodeo");}
    QString fileexcelo() { return QString("fa::fileexcelo");}
    QString fileimageo() { return QString("fa::fileimageo");}
    QString filemovieo() { return QString("fa::filemovieo");}
    QString fileo() { return QString("fa::fileo");}
    QString filepdfo() { return QString("fa::filepdfo");}
    QString filephotoo() { return QString("fa::filephotoo");}
    QString filepictureo() { return QString("fa::filepictureo");}
    QString filepowerpoint() { return QString("fa::filepowerpointo");}
    QString filesoundo() { return QString("fa::filesoundo");}
    QString filetext() { return QString("fa::filetext");}
    QString filetexto() { return QString("fa::filetexto");}
    QString filevideoo() { return QString("fa::filevideoo");}
    QString filewordo() { return QString("fa::filewordo");}
    QString filezipo() { return QString("fa::filezipo");}
    QString fileso() { return QString("fa::fileso");}
    QString film() { return QString("fa::film");}
    QString filter() { return QString("fa::filter");}
    QString fire() { return QString("fa::fire");}
    QString fireextinguishe() { return QString("fa::fireextinguisher");}
    QString firefox() { return QString("fa::firefox");}
    QString firstorder() { return QString("fa::firstorder");}
    QString flag() { return QString("fa::flag");}
    QString flagcheckere() { return QString("fa::flagcheckered");}
    QString flago() { return QString("fa::flago");}
    QString flash() { return QString("fa::flash");}
    QString flask() { return QString("fa::flask");}
    QString flickr() { return QString("fa::flickr");}
    QString floppyo() { return QString("fa::floppyo");}
    QString folder() { return QString("fa::folder");}
    QString foldero() { return QString("fa::foldero");}
    QString folderopen() { return QString("fa::folderopen");}
    QString folderopeno() { return QString("fa::folderopeno");}
    QString font() { return QString("fa::font");}
    QString fontawesome() { return QString("fa::fontawesome");}
    QString fonticons() { return QString("fa::fonticons");}
    QString fortawesome() { return QString("fa::fortawesome");}
    QString forumbee() { return QString("fa::forumbee");}
    QString forward() { return QString("fa::forward");}
    QString foursQuare() { return QString("fa::foursQuare");}
    QString freecodecamp() { return QString("fa::freecodecamp");}
    QString frowno() { return QString("fa::frowno");}
    QString futbolo() { return QString("fa::futbolo");}
    QString gamepad() { return QString("fa::gamepad");}
    QString gavel() { return QString("fa::gavel");}
    QString gbp() { return QString("fa::gbp");}
    QString ge() { return QString("fa::ge");}
    QString gear() { return QString("fa::gear");}
    QString gears() { return QString("fa::gears");}
    QString genderless() { return QString("fa::genderless");}
    QString getpocket() { return QString("fa::getpocket");}
    QString gg() { return QString("fa::gg");}
    QString ggcircle() { return QString("fa::ggcircle");}
    QString gift() { return QString("fa::gift");}
    QString git() { return QString("fa::git");}
    QString gitsQuare() { return QString("fa::gitsQuare");}
    QString github() { return QString("fa::github");}
    QString githubalt() { return QString("fa::githubalt");}
    QString githubsQuare() { return QString("fa::githubsQuare");}
    QString gitlab() { return QString("fa::gitlab");}
    QString gittip() { return QString("fa::gittip");}
    QString glass() { return QString("fa::glass");}
    QString glide() { return QString("fa::glide");}
    QString glideg() { return QString("fa::glideg");}
    QString globe() { return QString("fa::globe");}
    QString google() { return QString("fa::google");}
    QString googleplus() { return QString("fa::googleplus");}
    QString googlepluscircl() { return QString("fa::googlepluscircle");}
    QString googleplusofficia() { return QString("fa::googleplusofficial");}
    QString googleplussQuar() { return QString("fa::googleplussQuare");}
    QString googlewallet() { return QString("fa::googlewallet");}
    QString graduationca() { return QString("fa::graduationcap");}
    QString gratipay() { return QString("fa::gratipay");}
    QString grav() { return QString("fa::grav");}
    QString group() { return QString("fa::group");}
    QString hsQuare() { return QString("fa::hsQuare");}
    QString hackernews() { return QString("fa::hackernews");}
    QString handgrabo() { return QString("fa::handgrabo");}
    QString handlizardo() { return QString("fa::handlizardo");}
    QString handodown() { return QString("fa::handodown");}
    QString handoleft() { return QString("fa::handoleft");}
    QString handoright() { return QString("fa::handoright");}
    QString handoup() { return QString("fa::handoup");}
    QString handpapero() { return QString("fa::handpapero");}
    QString handpeaceo() { return QString("fa::handpeaceo");}
    QString handpointero() { return QString("fa::handpointero");}
    QString handrocko() { return QString("fa::handrocko");}
    QString handscissors() { return QString("fa::handscissorso");}
    QString handspocko() { return QString("fa::handspocko");}
    QString handstopo() { return QString("fa::handstopo");}
    QString handshakeo() { return QString("fa::handshakeo");}
    QString hardofhearin() { return QString("fa::hardofhearing");}
    QString hashtag() { return QString("fa::hashtag");}
    QString hddo() { return QString("fa::hddo");}
    QString header() { return QString("fa::header");}
    QString headphones() { return QString("fa::headphones");}
    QString heart() { return QString("fa::heart");}
    QString hearto() { return QString("fa::hearto");}
    QString heartbeat() { return QString("fa::heartbeat");}
    QString history() { return QString("fa::history");}
    QString home() { return QString("fa::home");}
    QString hospitalo() { return QString("fa::hospitalo");}
    QString hotel() { return QString("fa::hotel");}
    QString hourglass() { return QString("fa::hourglass");}
    QString hourglass1() { return QString("fa::hourglass1");}
    QString hourglass2() { return QString("fa::hourglass2");}
    QString hourglass3() { return QString("fa::hourglass3");}
    QString hourglassend() { return QString("fa::hourglassend");}
    QString hourglasshal() { return QString("fa::hourglasshalf");}
    QString hourglasso() { return QString("fa::hourglasso");}
    QString hourglassstar() { return QString("fa::hourglassstart");}
    QString houzz() { return QString("fa::houzz");}
    QString html5() { return QString("fa::html5");}
    QString icursor() { return QString("fa::icursor");}
    QString idbadge() { return QString("fa::idbadge");}
    QString idcard() { return QString("fa::idcard");}
    QString idcardo() { return QString("fa::idcardo");}
    QString ils() { return QString("fa::ils");}
    QString image() { return QString("fa::image");}
    QString imdb() { return QString("fa::imdb");}
    QString inbox() { return QString("fa::inbox");}
    QString indent() { return QString("fa::indent");}
    QString industry() { return QString("fa::industry");}
    QString info() { return QString("fa::info");}
    QString infocircle() { return QString("fa::infocircle");}
    QString inr() { return QString("fa::inr");}
    QString instagram() { return QString("fa::instagram");}
    QString institution() { return QString("fa::institution");}
    QString internetexplore() { return QString("fa::internetexplorer");}
    QString intersex() { return QString("fa::intersex");}
    QString ioxhost() { return QString("fa::ioxhost");}
    QString italic() { return QString("fa::italic");}
    QString joomla() { return QString("fa::joomla");}
    QString jpy() { return QString("fa::jpy");}
    QString jsfiddle() { return QString("fa::jsfiddle");}
    QString key() { return QString("fa::key");}
    QString keyboardo() { return QString("fa::keyboardo");}
    QString krw() { return QString("fa::krw");}
    QString language() { return QString("fa::language");}
    QString laptop() { return QString("fa::laptop");}
    QString lastfm() { return QString("fa::lastfm");}
    QString lastfmsQuare() { return QString("fa::lastfmsQuare");}
    QString leaf() { return QString("fa::leaf");}
    QString leanpub() { return QString("fa::leanpub");}
    QString legal() { return QString("fa::legal");}
    QString lemono() { return QString("fa::lemono");}
    QString leveldown() { return QString("fa::leveldown");}
    QString levelup() { return QString("fa::levelup");}
    QString lifebouy() { return QString("fa::lifebouy");}
    QString lifebuoy() { return QString("fa::lifebuoy");}
    QString lifering() { return QString("fa::lifering");}
    QString lifesaver() { return QString("fa::lifesaver");}
    QString lightbulbo() { return QString("fa::lightbulbo");}
    QString linechart() { return QString("fa::linechart");}
    QString link() { return QString("fa::link");}
    QString linkedin() { return QString("fa::linkedin");}
    QString linkedinsQuar() { return QString("fa::linkedinsQuare");}
    QString linode() { return QString("fa::linode");}
    QString fa_linux() { return QString("fa::fa_linux");}
    QString list() { return QString("fa::list");}
    QString listalt() { return QString("fa::listalt");}
    QString listol() { return QString("fa::listol");}
    QString listul() { return QString("fa::listul");}
    QString locationarro() { return QString("fa::locationarrow");}
    QString lock() { return QString("fa::lock");}
    QString longarrowdow() { return QString("fa::longarrowdown");}
    QString longarrowlef() { return QString("fa::longarrowleft");}
    QString longarrowrigh() { return QString("fa::longarrowright");}
    QString longarrowup() { return QString("fa::longarrowup");}
    QString lowvision() { return QString("fa::lowvision");}
    QString magic() { return QString("fa::magic");}
    QString magnet() { return QString("fa::magnet");}
    QString mailforward() { return QString("fa::mailforward");}
    QString mailreply() { return QString("fa::mailreply");}
    QString mailreplyall() { return QString("fa::mailreplyall");}
    QString male() { return QString("fa::male");}
    QString map() { return QString("fa::map");}
    QString mapmarker() { return QString("fa::mapmarker");}
    QString mapo() { return QString("fa::mapo");}
    QString mappin() { return QString("fa::mappin");}
    QString mapsigns() { return QString("fa::mapsigns");}
    QString mars() { return QString("fa::mars");}
    QString marsdouble() { return QString("fa::marsdouble");}
    QString marsstroke() { return QString("fa::marsstroke");}
    QString marsstrokeh() { return QString("fa::marsstrokeh");}
    QString marsstrokev() { return QString("fa::marsstrokev");}
    QString maxcdn() { return QString("fa::maxcdn");}
    QString meanpath() { return QString("fa::meanpath");}
    QString medium() { return QString("fa::medium");}
    QString medkit() { return QString("fa::medkit");}
    QString meetup() { return QString("fa::meetup");}
    QString meho() { return QString("fa::meho");}
    QString mercury() { return QString("fa::mercury");}
    QString microchip() { return QString("fa::microchip");}
    QString microphone() { return QString("fa::microphone");}
    QString microphoneslas() { return QString("fa::microphoneslash");}
    QString minus() { return QString("fa::minus");}
    QString minuscircle() { return QString("fa::minuscircle");}
    QString minussQuare() { return QString("fa::minussQuare");}
    QString minussQuareo() { return QString("fa::minussQuareo");}
    QString mixcloud() { return QString("fa::mixcloud");}
    QString mobile() { return QString("fa::mobile");}
    QString mobilephone() { return QString("fa::mobilephone");}
    QString modx() { return QString("fa::modx");}
    QString money() { return QString("fa::money");}
    QString moono() { return QString("fa::moono");}
    QString mortarboard() { return QString("fa::mortarboard");}
    QString motorcycle() { return QString("fa::motorcycle");}
    QString mousepointer() { return QString("fa::mousepointer");}
    QString music() { return QString("fa::music");}
    QString navicon() { return QString("fa::navicon");}
    QString neuter() { return QString("fa::neuter");}
    QString newspapero() { return QString("fa::newspapero");}
    QString objectgroup() { return QString("fa::objectgroup");}
    QString objectungrou() { return QString("fa::objectungroup");}
    QString odnoklassnik() { return QString("fa::odnoklassniki");}
    QString odnoklassnikisQuare() {return  QString("fa::odnoklassnikisQuare");}
    QString opencart() { return QString("fa::opencart");}
    QString openid() { return QString("fa::openid");}
    QString opera() { return QString("fa::opera");}
    QString optinmonster() { return QString("fa::optinmonster");}
    QString outdent() { return QString("fa::outdent");}
    QString pagelines() { return QString("fa::pagelines");}
    QString paintbrush() { return QString("fa::paintbrush");}
    QString paperplane() { return QString("fa::paperplane");}
    QString paperplaneo() { return QString("fa::paperplaneo");}
    QString paperclip() { return QString("fa::paperclip");}
    QString paragraph() { return QString("fa::paragraph");}
    QString paste() { return QString("fa::paste");}
    QString pause() { return QString("fa::pause");}
    QString pausecircle() { return QString("fa::pausecircle");}
    QString pausecircleo() { return QString("fa::pausecircleo");}
    QString paw() { return QString("fa::paw");}
    QString paypal() { return QString("fa::paypal");}
    QString pencil() { return QString("fa::pencil");}
    QString pencilsQuare() { return QString("fa::pencilsQuare");}
    QString pencilsQuare0() { return QString("fa::pencilsQuareo");}
    QString percent() { return QString("fa::percent");}
    QString phone() { return QString("fa::phone");}
    QString phonesQuare() { return QString("fa::phonesQuare");}
    QString photo() { return QString("fa::photo");}
    QString pictureo() { return QString("fa::pictureo");}
    QString piechart() { return QString("fa::piechart");}
    QString piedpiper() { return QString("fa::piedpiper");}
    QString piedpiperalt() { return QString("fa::piedpiperalt");}
    QString piedpiperpp() { return QString("fa::piedpiperpp");}
    QString pinterest() { return QString("fa::pinterest");}
    QString pinterestp() { return QString("fa::pinterestp");}
    QString pinterestsQuar() { return QString("fa::pinterestsQuare");}
    QString plane() { return QString("fa::plane");}
    QString play() { return QString("fa::play");}
    QString playcircle() { return QString("fa::playcircle");}
    QString playcircleo() { return QString("fa::playcircleo");}
    QString plug() { return QString("fa::plug");}
    QString plus() { return QString("fa::plus");}
    QString pluscircle() { return QString("fa::pluscircle");}
    QString plussQuare() { return QString("fa::plussQuare");}
    QString plussQuareo() { return QString("fa::plussQuareo");}
    QString podcast() { return QString("fa::podcast");}
    QString poweroff() { return QString("fa::poweroff");}
    QString print() { return QString("fa::print");}
    QString producthunt() { return QString("fa::producthunt");}
    QString puzzlepiece() { return QString("fa::puzzlepiece");}
    QString QQ() { return QString("fa::qq");}
    QString Qrcode() { return QString("fa::qrcode");}
    QString Question() { return QString("fa::question");}
    QString Questioncircl() { return QString("fa::questioncircle");}
    QString Questioncircle() { return QString("fa::questioncircleo");}
    QString Quora() { return QString("fa::quora");}
    QString Quoteleft() { return QString("fa::quoteleft");}
    QString Quoteright() { return QString("fa::quoteright");}
    QString ra() { return QString("fa::ra");}
    QString random() { return QString("fa::random");}
    QString ravelry() { return QString("fa::ravelry");}
    QString rebel() { return QString("fa::rebel");}
    QString recycle() { return QString("fa::recycle");}
    QString reddit() { return QString("fa::reddit");}
    QString redditalien() { return QString("fa::redditalien");}
    QString redditsQuare() { return QString("fa::redditsQuare");}
    QString refresh() { return QString("fa::refresh");}
    QString registered() { return QString("fa::registered");}
    QString remove() { return QString("fa::remove");}
    QString renren() { return QString("fa::renren");}
    QString reorder() { return QString("fa::reorder");}
    QString repeat() { return QString("fa::repeat");}
    QString reply() { return QString("fa::reply");}
    QString replyall() { return QString("fa::replyall");}
    QString resistance() { return QString("fa::resistance");}
    QString retweet() { return QString("fa::retweet");}
    QString rmb() { return QString("fa::rmb");}
    QString road() { return QString("fa::road");}
    QString rocket() { return QString("fa::rocket");}
    QString rotateleft() { return QString("fa::rotateleft");}
    QString rotateright() { return QString("fa::rotateright");}
    QString rouble() { return QString("fa::rouble");}
    QString rss() { return QString("fa::rss");}
    QString rsssQuare() { return QString("fa::rsssQuare");}
    QString rub() { return QString("fa::rub");}
    QString ruble() { return QString("fa::ruble");}
    QString rupee() { return QString("fa::rupee");}
    QString s15() { return QString("fa::s15");}
    QString safari() { return QString("fa::safari");}
    QString save() { return QString("fa::save");}
    QString scissors() { return QString("fa::scissors");}
    QString scribd() { return QString("fa::scribd");}
    QString search() { return QString("fa::search");}
    QString searchminus() { return QString("fa::searchminus");}
    QString searchplus() { return QString("fa::searchplus");}
    QString sellsy() { return QString("fa::sellsy");}
    QString send() { return QString("fa::send");}
    QString sendo() { return QString("fa::sendo");}
    QString server() { return QString("fa::server");}
    QString share() { return QString("fa::share");}
    QString sharealt() { return QString("fa::sharealt");}
    QString sharealtsQuar() { return QString("fa::sharealtsQuare");}
    QString sharesQuare() { return QString("fa::sharesQuare");}
    QString sharesQuareo() { return QString("fa::sharesQuareo");}
    QString shekel() { return QString("fa::shekel");}
    QString sheQel() { return QString("fa::sheQel");}
    QString shield() { return QString("fa::shield");}
    QString ship() { return QString("fa::ship");}
    QString shirtsinbulk() { return QString("fa::shirtsinbulk");}
    QString shoppingbag() { return QString("fa::shoppingbag");}
    QString shoppingbaske() { return QString("fa::shoppingbasket");}
    QString shoppingcart() { return QString("fa::shoppingcart");}
    QString shower() { return QString("fa::shower");}
    QString signin() { return QString("fa::signin");}
    QString signlanguage() { return QString("fa::signlanguage");}
    QString signout() { return QString("fa::signout");}
    QString signal() { return QString("fa::signal");}
    QString signing() { return QString("fa::signing");}
    QString simplybuilt() { return QString("fa::simplybuilt");}
    QString sitemap() { return QString("fa::sitemap");}
    QString skyatlas() { return QString("fa::skyatlas");}
    QString skype() { return QString("fa::skype");}
    QString slack() { return QString("fa::slack");}
    QString sliders() { return QString("fa::sliders");}
    QString slideshare() { return QString("fa::slideshare");}
    QString smileo() { return QString("fa::smileo");}
    QString snapchat() { return QString("fa::snapchat");}
    QString snapchatghos() { return QString("fa::snapchatghost");}
    QString snapchatsQuar() { return QString("fa::snapchatsQuare");}
    QString snowflakeo() { return QString("fa::snowflakeo");}
    QString soccerballo() { return QString("fa::soccerballo");}
    QString sort() { return QString("fa::sort");}
    QString sortalphaasc() { return QString("fa::sortalphaasc");}
    QString sortalphades() { return QString("fa::sortalphadesc");}
    QString sortamountas() { return QString("fa::sortamountasc");}
    QString sortamountdes() { return QString("fa::sortamountdesc");}
    QString sortasc() { return QString("fa::sortasc");}
    QString sortdesc() { return QString("fa::sortdesc");}
    QString sortdown() { return QString("fa::sortdown");}
    QString sortnumericas() { return QString("fa::sortnumericasc");}
    QString sortnumericdes() { return QString("fa::sortnumericdesc");}
    QString sortup() { return QString("fa::sortup");}
    QString soundcloud() { return QString("fa::soundcloud");}
    QString spaceshuttle() { return QString("fa::spaceshuttle");}
    QString spinner() { return QString("fa::spinner");}
    QString spoon() { return QString("fa::spoon");}
    QString spotify() { return QString("fa::spotify");}
    QString sQuare() { return QString("fa::sQuare");}
    QString sQuareo() { return QString("fa::sQuareo");}
    QString stackexchang() { return QString("fa::stackexchange");}
    QString stackoverflo() { return QString("fa::stackoverflow");}
    QString star() { return QString("fa::star");}
    QString starhalf() { return QString("fa::starhalf");}
    QString starhalfempt() { return QString("fa::starhalfempty");}
    QString starhalffull() { return QString("fa::starhalffull");}
    QString starhalfo() { return QString("fa::starhalfo");}
    QString staro() { return QString("fa::staro");}
    QString steam() { return QString("fa::steam");}
    QString steamsQuare() { return QString("fa::steamsQuare");}
    QString stepbackward() { return QString("fa::stepbackward");}
    QString stepforward() { return QString("fa::stepforward");}
    QString stethoscope() { return QString("fa::stethoscope");}
    QString stickynote() { return QString("fa::stickynote");}
    QString stickynoteo() { return QString("fa::stickynoteo");}
    QString stop() { return QString("fa::stop");}
    QString stopcircle() { return QString("fa::stopcircle");}
    QString stopcircleo() { return QString("fa::stopcircleo");}
    QString streetview() { return QString("fa::streetview");}
    QString strikethroug() { return QString("fa::strikethrough");}
    QString stumbleupon() { return QString("fa::stumbleupon");}
    QString stumbleuponcircl() { return QString("fa::stumbleuponcircle");}
    QString subscript() { return QString("fa::subscript");}
    QString subway() { return QString("fa::subway");}
    QString suitcase() { return QString("fa::suitcase");}
    QString suno() { return QString("fa::suno");}
    QString superpowers() { return QString("fa::superpowers");}
    QString superscript() { return QString("fa::superscript");}
    QString support() { return QString("fa::support");}
    QString table() { return QString("fa::table");}
    QString tablet() { return QString("fa::tablet");}
    QString tachometer() { return QString("fa::tachometer");}
    QString tag() { return QString("fa::tag");}
    QString tags() { return QString("fa::tags");}
    QString tasks() { return QString("fa::tasks");}
    QString taxi() { return QString("fa::taxi");}
    QString telegram() { return QString("fa::telegram");}
    QString television() { return QString("fa::television");}
    QString tencentweibo() { return QString("fa::tencentweibo");}
    QString terminal() { return QString("fa::terminal");}
    QString textheight() { return QString("fa::textheight");}
    QString textwidth() { return QString("fa::textwidth");}
    QString th() { return QString("fa::th");}
    QString thlarge() { return QString("fa::thlarge");}
    QString thlist() { return QString("fa::thlist");}
    QString themeisle() { return QString("fa::themeisle");}
    QString thermometer() { return QString("fa::thermometer");}
    QString thermometer0() { return QString("fa::thermometer0");}
    QString thermometer1() { return QString("fa::thermometer1");}
    QString thermometer2() { return QString("fa::thermometer2");}
    QString thermometer3() { return QString("fa::thermometer3");}
    QString thermometer4() { return QString("fa::thermometer4");}
    QString thermometerempt() { return QString("fa::thermometerempty");}
    QString thermometerful() { return QString("fa::thermometerfull");}
    QString thermometerhal() { return QString("fa::thermometerhalf");}
    QString thermometerQuarte() { return QString("fa::thermometerQuarter");}
    QString thermometerthreeQuarters() {return QString("fa::thermometerthreeQuarters");}
    QString thumbtack() { return QString("fa::thumbtack");}
    QString thumbsdown() { return QString("fa::thumbsdown");}
    QString thumbsodown() { return QString("fa::thumbsodown");}
    QString thumbsoup() { return QString("fa::thumbsoup");}
    QString thumbsup() { return QString("fa::thumbsup");}
    QString ticket() { return QString("fa::ticket");}
    QString times() { return QString("fa::times");}
    QString timescircle() { return QString("fa::timescircle");}
    QString timescircleo() { return QString("fa::timescircleo");}
    QString timesrectangl() { return QString("fa::timesrectangle");}
    QString timesrectangle() { return QString("fa::timesrectangleo");}
    QString tint() { return QString("fa::tint");}
    QString toggledown() { return QString("fa::toggledown");}
    QString toggleleft() { return QString("fa::toggleleft");}
    QString toggleoff() { return QString("fa::toggleoff");}
    QString toggleon() { return QString("fa::toggleon");}
    QString toggleright() { return QString("fa::toggleright");}
    QString toggleup() { return QString("fa::toggleup");}
    QString trademark() { return QString("fa::trademark");}
    QString train() { return QString("fa::train");}
    QString transgender() { return QString("fa::transgender");}
    QString transgenderal() { return QString("fa::transgenderalt");}
    QString trash() { return QString("fa::trash");}
    QString trasho() { return QString("fa::trasho");}
    QString tree() { return QString("fa::tree");}
    QString trello() { return QString("fa::trello");}
    QString tripadvisor() { return QString("fa::tripadvisor");}
    QString trophy() { return QString("fa::trophy");}
    QString truck() { return QString("fa::truck");}
    QString fa_try() { return QString("fa::fa_try");}
    QString tty() { return QString("fa::tty");}
    QString tumblr() { return QString("fa::tumblr");}
    QString tumblrsQuare() { return QString("fa::tumblrsQuare");}
    QString turkishlira() { return QString("fa::turkishlira");}
    QString tv() { return QString("fa::tv");}
    QString twitch() { return QString("fa::twitch");}
    QString twitter() { return QString("fa::twitter");}
    QString twittersQuar() { return QString("fa::twittersQuare");}
    QString umbrella() { return QString("fa::umbrella");}
    QString underline() { return QString("fa::underline");}
    QString undo() { return QString("fa::undo");}
    QString universalacces() { return QString("fa::universalaccess");}
    QString university() { return QString("fa::university");}
    QString unlink() { return QString("fa::unlink");}
    QString unlock() { return QString("fa::unlock");}
    QString unlockalt() { return QString("fa::unlockalt");}
    QString unsorted() { return QString("fa::unsorted");}
    QString upload() { return QString("fa::upload");}
    QString usb() { return QString("fa::usb");}
    QString usd() { return QString("fa::usd");}
    QString user() { return QString("fa::user");}
    QString usercircle() { return QString("fa::usercircle");}
    QString usercircleo() { return QString("fa::usercircleo");}
    QString usermd() { return QString("fa::usermd");}
    QString usero() { return QString("fa::usero");}
    QString userplus() { return QString("fa::userplus");}
    QString usersecret() { return QString("fa::usersecret");}
    QString usertimes() { return QString("fa::usertimes");}
    QString users() { return QString("fa::users");}
    QString vcard() { return QString("fa::vcard");}
    QString vcardo() { return QString("fa::vcardo");}
    QString venus() { return QString("fa::venus");}
    QString venusdouble() { return QString("fa::venusdouble");}
    QString venusmars() { return QString("fa::venusmars");}
    QString viacoin() { return QString("fa::viacoin");}
    QString viadeo() { return QString("fa::viadeo");}
    QString viadeosQuare() { return QString("fa::viadeosQuare");}
    QString videocamera() { return QString("fa::videocamera");}
    QString vimeo() { return QString("fa::vimeo");}
    QString vimeosQuare() { return QString("fa::vimeosQuare");}
    QString vine() { return QString("fa::vine");}
    QString vk() { return QString("fa::vk");}
    QString volumecontrolphon() { return QString("fa::volumecontrolphone");}
    QString volumedown() { return QString("fa::volumedown");}
    QString volumeoff() { return QString("fa::volumeoff");}
    QString volumeup() { return QString("fa::volumeup");}
    QString warning() { return QString("fa::warning");}
    QString wechat() { return QString("fa::wechat");}
    QString weibo() { return QString("fa::weibo");}
    QString weixin() { return QString("fa::weixin");}
    QString whatsapp() { return QString("fa::whatsapp");}
    QString wheelchair() { return QString("fa::wheelchair");}
    QString wheelchairal() { return QString("fa::wheelchairalt");}
    QString wifi() { return QString("fa::wifi");}
    QString wikipediaw() { return QString("fa::wikipediaw");}
    QString windowclose() { return QString("fa::windowclose");}
    QString windowcloseo() { return QString("fa::windowcloseo");}
    QString windowmaximiz() { return QString("fa::windowmaximize");}
    QString windowminimiz() { return QString("fa::windowminimize");}
    QString windowrestor() { return QString("fa::windowrestore");}
    QString windows() { return QString("fa::windows");}
    QString won() { return QString("fa::won");}
    QString wordpress() { return QString("fa::wordpress");}
    QString wpbeginner() { return QString("fa::wpbeginner");}
    QString wpexplorer() { return QString("fa::wpexplorer");}
    QString wpforms() { return QString("fa::wpforms");}
    QString wrench() { return QString("fa::wrench");}
    QString xing() { return QString("fa::xing");}
    QString xingsQuare() { return QString("fa::xingsQuare");}
    QString ycombinator() { return QString("fa::ycombinator");}
    QString ycombinatorsQuar() { return QString("fa::ycombinatorsQuare");}
    QString yahoo() { return QString("fa::yahoo");}
    QString yc() { return QString("fa::yc");}
    QString ycsQuare() { return QString("fa::ycsQuare");}
    QString yelp() { return QString("fa::yelp");}
    QString yen() { return QString("fa::yen");}
    QString yoast() { return QString("fa::yoast");}
    QString youtube() { return QString("fa::youtube");}
    QString youtubeplay() { return QString("fa::youtubeplay");}
    QString youtubesQuar() { return QString("fa::youtubesQuare");}
signals:
    void canNotChangeIcon();  //just to remove qml errors
private:
    QtAwesome* awesome ;
};

#endif // ICONS_H
