const fetch = require('node-fetch');

const profiles = [
  {
    token: "ltoken=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx; ltuid=xxxxxx;",
    genshin: false,
    honkai_star_rail: true,
    honkai_3: false,
    accountName: "您账户的名字"
  }
];

const telegram_notify = true;
const myTelegramID = "1351xxxxxx";
const telegramBotToken = "";

const urlDict = {
  Genshin: 'https://sg-hk4e-api.hoyolab.com/event/sol/sign?lang=zh-tw&act_id=e202102251931481',
  Star_Rail: 'https://sg-public-api.hoyolab.com/event/luna/os/sign?lang=zh-tw&act_id=e202303301540311',
  Honkai_3: 'https://sg-public-api.hoyolab.com/event/mani/sign?lang=zh-tw&act_id=e202110291205111'
};

function main() {
  const messages = Promise.all(profiles.map(autoSignFunction));
  messages.then((messages) => {
    const hoyolabResp = `${messages.join('\n\n')}`;
  
    if (telegram_notify === true) {
      if (telegramBotToken && myTelegramID) {
        postWebhook(hoyolabResp);
      }
    }
  });
}

function autoSignFunction({ token, genshin, honkai_star_rail, honkai_3, accountName }) {
  const urls = [];

  if (genshin) urls.push(urlDict.Genshin);
  if (honkai_star_rail) urls.push(urlDict.Star_Rail);
  if (honkai_3) urls.push(urlDict.Honkai_3);

  const header = {
    Cookie: token
  };

  const options = {
    method: 'POST',
    headers: header
  };

  let response = `${accountName} 的自動簽到作業已完成`;

  return Promise.all(
    urls.map(url =>
      fetch(url, options)
        .then(response => response.json())
        .then(jsonResponse => {
          const checkInResult = jsonResponse.message;
          const enGameName = Object.keys(urlDict).find(key => urlDict[key] === url);
          let gameName;
          switch (enGameName) {
            case 'Genshin':
              gameName = '原神';
              break;
            case 'Star_Rail':
              gameName = '星穹鐵道';
              break;
            case 'Honkai_3':
              gameName = '崩壞3rd';
              break;
          }
          response += `\n${gameName}： ${checkInResult}`;
        })
    )
  ).then(() => response);
}

function postWebhook(data) {
  const payload = JSON.stringify({
    chat_id: myTelegramID,
    text: data,
    parse_mode: 'HTML'
  });

  const options = {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: payload
  };

  fetch(`https://api.telegram.org/bot${telegramBotToken}/sendMessage`, options);
}

main();
