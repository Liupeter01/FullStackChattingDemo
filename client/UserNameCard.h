#ifndef USERNAMECARD_H
#define USERNAMECARD_H
#include <QString>
#include <memory>
#include <cstdlib>

enum class Sex { Male, Female };

/*record the critical info of the user*/
struct UserNameCard {
  UserNameCard(std::size_t uid, QString avator_path, QString nickname,
               QString desc, Sex sex)
      : m_sex(sex), m_uid(uid), m_nickname(nickname), m_description(desc),
        m_avatorPath(avator_path) {}

  Sex m_sex;
  std::size_t m_uid;
  QString m_avatorPath;
  QString m_nickname;
  QString m_description;
};

#endif // USERNAMECARD_H
